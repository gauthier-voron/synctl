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


#ifndef _INCLUDE_SYNCTL_CONFIGURATIONINIT_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATIONINIT_HXX_


#include <string>
#include <vector>

#include "synctl/ui/ConfigurationCommand.hxx"
#include "synctl/ui/OptionBoolean.hxx"


namespace synctl {


class ConfigurationBase;


class ConfigurationInit : public ConfigurationCommand
{
	OptionBoolean  _optionForce = OptionBoolean("force", 'f');
	bool           _hasOperand;
	std::string    _operandServerPath;


 public:
	ConfigurationInit() = default;
	ConfigurationInit(ConfigurationBase *inner);
	ConfigurationInit(const ConfigurationInit &other) = default;

	ConfigurationInit &operator=(const ConfigurationInit &other) = default;


	const OptionBoolean &optionForce() const;

	bool hasServerPath() const;

	bool force() const;

	const std::string &serverPath() const;


	virtual size_t getOperands(const std::vector<std::string> &args);

	virtual void getOptions(std::vector<Option *> *dest);

	using ConfigurationCommand::getOptions;


	static int main(ConfigurationBase *base,
			const std::vector<std::string> &args);
};


}


#endif
