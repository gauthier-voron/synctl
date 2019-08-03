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


#include "synctl/ui/ConfigurationBase.hxx"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "synctl/default.hxx"
#include "synctl/ui/OptionLambda.hxx"
#include "synctl/ui/OptionString.hxx"
#include "synctl/ui/ProfileSeeker.hxx"


using std::cout;
using std::endl;
using std::string;
using std::vector;
using synctl::ConfigurationBase;
using synctl::OptionBoolean;
using synctl::OptionLambda;
using synctl::OptionString;
using synctl::ProfileSeeker;


void ConfigurationBase::_displayVersion() const
{
	cout << SYNCTL_CONSTANT_NAME;
	cout << " ";

	cout << SYNCTL_CONSTANT_MAJOR_VERSION;
	cout << ".";
	cout << SYNCTL_CONSTANT_MINOR_VERSION;
	cout << ".";
	cout << SYNCTL_CONSTANT_VARIANT;
	cout << endl;

	cout << SYNCTL_CONSTANT_AUTHOR;
	cout << endl;

	cout << SYNCTL_CONSTANT_MAILTO;
	cout << endl;
}

ConfigurationBase::ConfigurationBase()
	: _optionVersion("version", 'V', [&]() {
		_displayVersion();
		::exit(0);
	})
{
}

const OptionString &ConfigurationBase::optionConfig() const
{
	return _optionConfig;
}

const OptionBoolean &ConfigurationBase::optionHelp() const
{
	return _optionHelp;
}

bool ConfigurationBase::hasCommand() const
{
	return _hasCommand;
}

string ConfigurationBase::config() const
{
	char *env;

	if (_optionConfig.affected())
		return _optionConfig.value();

	if ((env = getenv("SYNCTL_PROFILE_PATH")) != NULL)
		return env;

	return SYNCTL_DEFAULT_CONFIG;
}

bool ConfigurationBase::help() const
{
	return (_optionHelp.affected() > 0);
}

const string &ConfigurationBase::command() const
{
	return _operandCommand;
}

size_t ConfigurationBase::getOperands(const vector<string> &arguments)
{
	if (arguments.size() < 1) {
		_hasCommand = false;
		return 0;
	}

	_hasCommand = true;
	_operandCommand = arguments[0];

	return 1;
}

void ConfigurationBase::getOptions(vector<Option *> *dest)
{
	dest->push_back(&_optionConfig);
	dest->push_back(&_optionHelp);
	dest->push_back(&_optionVersion);
}

string ConfigurationBase::seekProfile(const string &name) const
{
	ProfileSeeker seeker;

	seeker.push(config());

	return seeker.seek(name);
}
