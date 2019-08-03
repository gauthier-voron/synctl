// Copyright 2019 Gauthier Voron
//
// This file is part of Synctl
//
// Synctl is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Fundation, either version 3 of the License, or (at your option) any later
// version.
//
// Synctl is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANDABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// Synctl. If not, see <https://www.gnu.org/licenses/>.
//


#include "synctl/ui/ConfigurationList.hxx"

#include <limits.h>
#include <unistd.h>

#include <memory>
#include <string>
#include <vector>

#include "synctl/default.hxx"
#include "synctl/io/Channel.hxx"
#include "synctl/io/SshChannel.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/ui/ArgumentParser.hxx"
#include "synctl/ui/Command.hxx"
#include "synctl/ui/ConfigurationBase.hxx"
#include "synctl/ui/ConfigurationCommand.hxx"
#include "synctl/ui/ConfigurationHelp.hxx"
#include "synctl/ui/FormatList.hxx"
#include "synctl/ui/FormatListHuman.hxx"
#include "synctl/ui/FormatListPorcelain.hxx"
#include "synctl/ui/OperandInvalidException.hxx"
#include "synctl/ui/OperandMissingException.hxx"
#include "synctl/ui/OperandUnexpectedException.hxx"
#include "synctl/ui/Option.hxx"
#include "synctl/ui/OptionString.hxx"
#include "synctl/ui/ProfileParser.hxx"


using std::make_unique;
using std::string;
using std::vector;
using std::unique_ptr;
using synctl::ArgumentParser;
using synctl::Channel;
using synctl::Command;
using synctl::ConfigurationBase;
using synctl::ConfigurationCommand;
using synctl::ConfigurationHelp;
using synctl::ConfigurationList;
using synctl::FormatList;
using synctl::FormatListHuman;
using synctl::FormatListPorcelain;
using synctl::OperandInvalidException;
using synctl::OperandMissingException;
using synctl::OperandUnexpectedException;
using synctl::Option;
using synctl::OptionString;
using synctl::ProfileParser;
using synctl::Protocol;
using synctl::SshChannel;


ConfigurationList::ConfigurationList(ConfigurationBase *inner)
	: ConfigurationCommand(inner)
{
}

const OptionString &ConfigurationList::optionBranch() const
{
	return _optionBranch;
}

const OptionString &ConfigurationList::optionCommand() const
{
	return _optionCommand;
}

const OptionString &ConfigurationList::optionFormat() const
{
	return _optionFormat;
}

const OptionString &ConfigurationList::optionServer() const
{
	return _optionServer;
}

const OptionString &ConfigurationList::optionTrunk() const
{
	return _optionTrunk;
}

bool ConfigurationList::hasProfile() const
{
	return _hasProfile;
}

const string &ConfigurationList::branch() const
{
	static string defaultValue;

	if (_optionBranch.affected())
		return _optionBranch.value();

	return defaultValue;
}

const string &ConfigurationList::command() const
{
	return _optionCommand.value();
}

unique_ptr<FormatList> ConfigurationList::format() const
{
	string value = SYNCTL_DEFAULT_LIST_FORMAT;

	if (_optionFormat.affected())
		value = _optionFormat.value();

	if (value == "human")
		return make_unique<FormatListHuman>();
	else if (value == "porcelain")
		return make_unique<FormatListPorcelain>();

	throw OperandInvalidException("format", value);
}

const string &ConfigurationList::server() const
{
	return _optionServer.value();
}

const string &ConfigurationList::trunk() const
{
	static string defaultValue = "*";

	if (_optionTrunk.affected())
		return _optionTrunk.value();

	return defaultValue;
}

string ConfigurationList::profile() const
{
	return _operandProfile;
}

size_t ConfigurationList::getOperands(const vector<string> &args)
{
	if (args.size() >= 1) {
		_hasProfile = true;
		_operandProfile = args[0];
		return 1;
	}

	return 0;
}

void ConfigurationList::getOptions(vector<Option *> *dest)
{
	ConfigurationCommand::getOptions(dest);

	dest->push_back(&_optionBranch);
	dest->push_back(&_optionCommand);
	dest->push_back(&_optionFormat);
	dest->push_back(&_optionServer);
	dest->push_back(&_optionTrunk);
}

static unique_ptr<Channel> __openSshChannel(ConfigurationList *config)
{
	SshChannel::OpenSettings settings;

	settings.location = config->server().substr(6);

	if (config->optionCommand().affected())
		settings.remoteCommand = Command::shlex(config->command());
	else
		settings.remoteCommand.clear();

	return SshChannel::open(settings);
}

static unique_ptr<Channel> __openChannel(ConfigurationList *config)
{
	if (config->server().find("ssh://") == 0)
		return __openSshChannel(config);

	return Channel::open(config->server());
}

static string __globToRegex(const string &glob) {
	static string escape = "^(){}.+";
	static string unescape = "*?[]";
	bool escaped = false;
	bool range = false;
	string ret = "^";
	size_t i, j;

	for (i = 0; i < glob.length(); i++) {
		if (escaped) {
			escaped = false;

			for (j = 0; j < unescape.length(); j++)
				if (glob[i] == unescape[j]) {
					ret.push_back('\\');
					ret.push_back(glob[i]);
					goto next;
				}

			ret.push_back('\\');
			ret.push_back('\\');
		}

		if (range) {
			range = false;

			if (glob[i] == '^') {
				ret.push_back('^');
				continue;
			}
		}

		if (glob[i] == '*') {
			ret += ".*";
			continue;
		}

		if (glob[i] == '?') {
			ret += ".";
			continue;
		}

		if (glob[i] == '\\') {
			escaped = true;
			continue;
		}

		if (glob[i] == '[') {
			range = true;
			ret.push_back(glob[i]);
			continue;
		}

		for (j = 0; j < escape.length(); j++) {
			if (glob[i] == escape[j]) {
				ret.push_back('\\');
				ret.push_back(escape[j]);
				goto next;
			}
		}

		ret.push_back(glob[i]);

	 next:
		continue;
	}

	if (escaped) {
		ret.push_back('\\');
		ret.push_back('\\');
	}

	ret.push_back('$');

	return ret;
}

static int __main(ConfigurationList *config)
{
	unique_ptr<Channel> channel = __openChannel(config);
	unique_ptr<FormatList> format = config->format();
	vector<Protocol::ListResult> lresults;
	Protocol::ListSettings lsettings;
	unique_ptr<Protocol> protocol;

	protocol = Protocol::clientHandcheck(channel.get());

	if (protocol == nullptr)
		return 1;

	lsettings.trunkRegex = __globToRegex(config->trunk());
	if (config->optionBranch().affected())
		lsettings.branchRegex = __globToRegex(config->branch());
	else
		lsettings.branchRegex.clear();
	lsettings.results = &lresults;

	protocol->list(lsettings);

	protocol->exit();
	channel->close();

	for (const Protocol::ListResult &result : lresults)
		format->load(result.trunk, result.name, result.date,
			     result.branches);

	format->flush();

	return 0;
}

int ConfigurationList::main(ConfigurationBase *c, const vector<string> &args)
{
	ConfigurationList conf = ConfigurationList(c);
	ArgumentParser aparser = ArgumentParser(&conf);
	ProfileParser pparser;
	vector<string> rem;
	string ppath;

	aparser.requireOrder() = false;
	rem = aparser.parse(args);

	if (conf.help())
		ConfigurationHelp::displayCommandHelp(c->command());

	if (rem.empty() == false)
		throw OperandUnexpectedException(rem[0]);

	if (conf.hasProfile()) {
		pparser = ProfileParser(&conf);
		ppath = conf.seekProfile(conf.profile());
		if (ppath.empty())
			throw OperandInvalidException
				("profile", conf.profile());
		pparser.parse(ppath);
	}

	if (conf.optionServer().affected() == false)
		throw OperandMissingException(conf.optionServer().longName());

	return __main(&conf);
}
