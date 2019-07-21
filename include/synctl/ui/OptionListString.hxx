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


#ifndef _INCLUDE_SYNCTL_OPTIONLISTSTRING_HXX_
#define _INCLUDE_SYNCTL_OPTIONLISTSTRING_HXX_


#include <string>
#include <vector>

#include "synctl/ui/Option.hxx"


namespace synctl {


class OptionListString : public Option
{
	std::vector<std::string>  _values;


 public:
	OptionListString(const std::string &longName);
	OptionListString(char shortName);
	OptionListString(const std::string &longName, char shortName);


	virtual bool acceptArgument() const override;

	virtual void affect() override;
	virtual void affect(const std::string &argument) override;


	const std::vector<std::string> &values() const;
};


}


#endif
