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


#include "synctl/ui/ConfigurationCommand.hxx"

#include <string>
#include <vector>

#include "synctl/ui/ConfigurationBase.hxx"
#include "synctl/ui/OptionBoolean.hxx"
#include "synctl/ui/OptionString.hxx"


using std::string;
using std::vector;
using synctl::ConfigurationCommand;
using synctl::OptionBoolean;
using synctl::OptionString;


ConfigurationCommand::ConfigurationCommand(ConfigurationBase *inner)
	: _inner(inner)
{
}

const OptionString &ConfigurationCommand::optionConfig() const
{
	return _inner->optionConfig();
}

const OptionBoolean &ConfigurationCommand::optionHelp() const
{
	return _inner->optionHelp();
}

string ConfigurationCommand::config() const
{
	return _inner->config();
}

bool ConfigurationCommand::help() const
{
	return _inner->help();
}

const string &ConfigurationCommand::command() const
{
	return _inner->command();
}

void ConfigurationCommand::getOptions(vector<Option *> *dest)
{
	_inner->getOptions(dest);
}

string ConfigurationCommand::seekProfile(const string &name) const
{
	return _inner->seekProfile(name);
}
