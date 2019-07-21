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


#ifndef _INCLUDE_SYNCTL_CONFIGURATIONCOMMAND_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATIONCOMMAND_HXX_


#include <string>

#include "synctl/ui/ConfigurationBase.hxx"
#include "synctl/ui/OptionBoolean.hxx"


namespace synctl {


class OptionString;


class ConfigurationCommand : public Configuration
{
	ConfigurationBase  *_inner;


 public:
	ConfigurationCommand() = default;
	ConfigurationCommand(ConfigurationBase *inner);
	ConfigurationCommand(const ConfigurationCommand &other) = default;

	ConfigurationCommand &operator=(const ConfigurationCommand &other) =
		default;


	const OptionString &optionConfig() const;

	const OptionBoolean &optionHelp() const;


	std::string config() const;

	bool help() const;


	const std::string &command() const;


	virtual void getOptions(std::vector<Option *> *dest);

	using Configuration::getOptions;


	std::string seekProfile(const std::string &name) const;
};


}


#endif
