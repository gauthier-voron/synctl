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


#ifndef _INCLUDE_SYNCTL_CONFIGURATIONPUSH_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATIONPUSH_HXX_


#include <string>
#include <vector>

#include "synctl/tree/FirstMatchFilter.hxx"
#include "synctl/ui/ConfigurationCommand.hxx"
#include "synctl/ui/OptionBoolean.hxx"
#include "synctl/ui/OptionLambda.hxx"
#include "synctl/ui/OptionString.hxx"


namespace synctl {


class ConfigurationBase;
class Filter;


class ConfigurationPush : public ConfigurationCommand
{
	OptionString   _optionBranch = OptionString("branch", 'b');
	OptionString   _optionCommand = OptionString("command", 'c');
	OptionLambda   _optionExclude;
	OptionLambda   _optionInclude;
	OptionString   _optionRoot = OptionString("root", 'r');
	OptionString   _optionServer = OptionString("server", 's');
	OptionString   _optionTrunk = OptionString("trunk", 't');
	OptionBoolean  _optionVerbose = OptionBoolean("verbose", 'v');

	bool           _hasProfile = false;
	std::string    _operandProfile;

	FirstMatchFilter  _filter;


 public:
	ConfigurationPush() = default;
	ConfigurationPush(ConfigurationBase *inner);
	ConfigurationPush(const ConfigurationPush &other) = default;

	ConfigurationPush &operator=(const ConfigurationPush &other) = default;


	const OptionString &optionBranch() const;
	const OptionString &optionCommand() const;
	const OptionString &optionRoot() const;
	const OptionString &optionServer() const;
	const OptionString &optionTrunk() const;
	const OptionBoolean &optionVerbose() const;
	bool hasProfile() const;

	const std::string &branch() const;
	const std::string &command() const;
	Filter *filter();
	const std::string &root() const;
	const std::string &server() const;
	const std::string &trunk() const;
	bool verbose() const;
	std::string profile() const;


	virtual size_t getOperands(const std::vector<std::string> &args);

	virtual void getOptions(std::vector<Option *> *dest);

	using ConfigurationCommand::getOptions;


	static int main(ConfigurationBase *base,
			const std::vector<std::string> &args);
};


}


#endif
