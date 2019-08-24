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


#include "synctl/ui/ConfigurationPush.hxx"

#include <limits.h>
#include <unistd.h>

#include <memory>
#include <string>
#include <vector>

#include "synctl/default.hxx"
#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/io/Channel.hxx"
#include "synctl/io/SshChannel.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/tree/FirstMatchFilter.hxx"
#include "synctl/tree/GlobPattern.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/ui/ArgumentParser.hxx"
#include "synctl/ui/Command.hxx"
#include "synctl/ui/ConfigurationBase.hxx"
#include "synctl/ui/ConfigurationHelp.hxx"
#include "synctl/ui/OperandInvalidException.hxx"
#include "synctl/ui/OperandMissingException.hxx"
#include "synctl/ui/OperandUnexpectedException.hxx"
#include "synctl/ui/OptionBoolean.hxx"
#include "synctl/ui/OptionLambda.hxx"
#include "synctl/ui/OptionString.hxx"
#include "synctl/ui/Path.hxx"
#include "synctl/ui/ProfileParser.hxx"


using std::make_unique;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::ArgumentParser;
using synctl::Channel;
using synctl::Command;
using synctl::ConfigurationBase;
using synctl::ConfigurationHelp;
using synctl::ConfigurationPush;
using synctl::Filter;
using synctl::FirstMatchFilter;
using synctl::GlobPattern;
using synctl::OperandInvalidException;
using synctl::OperandMissingException;
using synctl::OperandUnexpectedException;
using synctl::OptionBoolean;
using synctl::OptionLambda;
using synctl::OptionString;
using synctl::Path;
using synctl::ProfileParser;
using synctl::Protocol;
using synctl::SshChannel;


ConfigurationPush::ConfigurationPush(ConfigurationBase *inner)
	: ConfigurationCommand(inner)
	, _optionExclude("exclude", 'e', [&](auto ptrn) {
		_filter.append(make_unique<GlobPattern>(ptrn), Filter::Reject);
	})
	, _optionInclude("include", 'i', [&](auto ptrn) {
		_filter.append(make_unique<GlobPattern>(ptrn), Filter::Accept);
	})
{
}

const OptionString &ConfigurationPush::optionBranch() const
{
	return _optionBranch;
}

const OptionString &ConfigurationPush::optionCommand() const
{
	return _optionCommand;
}

const OptionString &ConfigurationPush::optionRoot() const
{
	return _optionRoot;
}

const OptionString &ConfigurationPush::optionServer() const
{
	return _optionServer;
}

const OptionString &ConfigurationPush::optionTrunk() const
{
	return _optionTrunk;
}

const OptionBoolean &ConfigurationPush::optionVerbose() const
{
	return _optionVerbose;
}

bool ConfigurationPush::hasProfile() const
{
	return _hasProfile;
}

const string &ConfigurationPush::branch() const
{
	static char hostname[HOST_NAME_MAX];
	static string defaultValue;
	static bool loaded = false;

	if (_optionBranch.affected())
		return _optionBranch.value();

	if (loaded == false) {
		gethostname(hostname, sizeof (hostname));
		defaultValue = string(hostname);
		loaded = true;
	}

	return defaultValue;
}

const string &ConfigurationPush::command() const
{
	return _optionCommand.value();
}

Filter *ConfigurationPush::filter()
{
	return &_filter;
}

const string &ConfigurationPush::root() const
{
	return _optionRoot.value();
}

const string &ConfigurationPush::server() const
{
	return _optionServer.value();
}

const string &ConfigurationPush::trunk() const
{
	static string defaultValue = SYNCTL_DEFAULT_TRUNK;

	if (_optionTrunk.affected())
		return _optionTrunk.value();

	return defaultValue;
}

bool ConfigurationPush::verbose() const
{
	return (_optionVerbose.affected() > 0);
}

string ConfigurationPush::profile() const
{
	return _operandProfile;
}

size_t ConfigurationPush::getOperands(const vector<string> &args)
{
	if (args.size() >= 1) {
		_hasProfile = true;
		_operandProfile = args[0];
		return 1;
	}

	return 0;
}

void ConfigurationPush::getOptions(vector<Option *> *dest)
{
	ConfigurationCommand::getOptions(dest);

	dest->push_back(&_optionBranch);
	dest->push_back(&_optionCommand);
	dest->push_back(&_optionExclude);
	dest->push_back(&_optionInclude);
	dest->push_back(&_optionRoot);
	dest->push_back(&_optionServer);
	dest->push_back(&_optionTrunk);
	dest->push_back(&_optionVerbose);
}

static unique_ptr<Channel> __openSshChannel(ConfigurationPush *config)
{
	SshChannel::OpenSettings settings;

	settings.location = config->server().substr(6);

	if (config->optionCommand().affected())
		settings.remoteCommand = Command::shlex(config->command());
	else
		settings.remoteCommand.clear();

	return SshChannel::open(settings);
}

static unique_ptr<Channel> __openChannel(ConfigurationPush *config)
{
	if (config->server().find("ssh://") == 0)
		return __openSshChannel(config);

	return Channel::open(config->server());
}

static int __main(ConfigurationPush *config)
{
	unique_ptr<Channel> channel = __openChannel(config);
	Protocol::PushSettings psettings;
	unique_ptr<Protocol> protocol;

	protocol = Protocol::clientHandcheck(channel.get());

	if (protocol == nullptr)
		return 1;

	psettings.localRoot = Path::resolvePath(config->root());
	psettings.branchName = config->branch();
	psettings.trunkName = config->trunk();
	psettings.snapshotName = nullptr;
	psettings.filter = config->filter();
	psettings.verbosity = config->verbose() ? 1 : 0;

	protocol->push(psettings);

	protocol->exit();
	channel->close();

	return 0;
}

int ConfigurationPush::main(ConfigurationBase *c, const vector<string> &args)
{
	ConfigurationPush conf = ConfigurationPush(c);
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

	if (conf.optionRoot().affected() == false)
		throw OperandMissingException(conf.optionRoot().longName());
	if (conf.optionServer().affected() == false)
		throw OperandMissingException(conf.optionServer().longName());

	return __main(&conf);
}
