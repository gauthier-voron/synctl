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


#include "synctl/ui/ConfigurationInit.hxx"

#include <string>
#include <vector>

#include "synctl/io/Directory.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/ui/ArgumentParser.hxx"
#include "synctl/ui/ConfigurationBase.hxx"
#include "synctl/ui/ConfigurationHelp.hxx"
#include "synctl/ui/OperandInvalidException.hxx"
#include "synctl/ui/OperandMissingException.hxx"
#include "synctl/ui/OperandUnexpectedException.hxx"
#include "synctl/ui/OptionBoolean.hxx"


using std::string;
using std::vector;
using synctl::ArgumentParser;
using synctl::ConfigurationBase;
using synctl::ConfigurationHelp;
using synctl::ConfigurationInit;
using synctl::Directory;
using synctl::OperandInvalidException;
using synctl::OperandMissingException;
using synctl::OperandUnexpectedException;
using synctl::OptionBoolean;
using synctl::Repository;


ConfigurationInit::ConfigurationInit(ConfigurationBase *inner)
	: ConfigurationCommand(inner)
{
}

const OptionBoolean &ConfigurationInit::optionForce() const
{
	return _optionForce;
}

bool ConfigurationInit::hasServerPath() const
{
	return _hasOperand;
}

bool ConfigurationInit::force() const
{
	return (_optionForce.affected() > 0);
}

const string &ConfigurationInit::serverPath() const
{
	return _operandServerPath;
}

size_t ConfigurationInit::getOperands(const vector<string> &args)
{
	if (args.size() < 1) {
		_hasOperand = false;
		return 0;
	}

	_hasOperand = true;
	_operandServerPath = args[0];

	return 1;
}

void ConfigurationInit::getOptions(vector<Option *> *dest)
{
	ConfigurationCommand::getOptions(dest);
	dest->push_back(&_optionForce);
}

static int __main(ConfigurationInit *config)
{
	Repository repository = Repository(config->serverPath());
	Directory directory = Directory(config->serverPath());

	if (directory.exists()) {
		if (config->force())
			directory.remove();
		else
			throw OperandInvalidException("server-path",
						      config->serverPath());
	}

	repository.initialize();

	return 0;
}

int ConfigurationInit::main(ConfigurationBase *c, const vector<string> &args)
{
	ConfigurationInit config = ConfigurationInit(c);
	ArgumentParser parser = ArgumentParser(&config);
	vector<string> rem = parser.parse(args);

	if (config.help())
		ConfigurationHelp::displayCommandHelp(c->command());

	if (config.hasServerPath() == false)
		throw OperandMissingException("server-path");

	if (rem.empty() == false)
		throw OperandUnexpectedException(rem[0]);

	return __main(&config);
}
