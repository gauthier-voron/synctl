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


#include "synctl/ui/OptionListString.hxx"

#include <string>
#include <vector>

#include "synctl/ui/Option.hxx"
#include "synctl/ui/OptionWithArgumentException.hxx"


using std::string;
using std::vector;
using synctl::Option;
using synctl::OptionListString;
using synctl::OptionWithArgumentException;


OptionListString::OptionListString(const string &longName)
	: Option(longName)
{
}

OptionListString::OptionListString(char shortName)
	: Option(shortName)
{
}

OptionListString::OptionListString(const string &longName, char shortName)
	: Option(longName, shortName)
{
}

bool OptionListString::acceptArgument() const
{
	return true;
}

void OptionListString::affect()
{
	throw OptionWithArgumentException();
}

void OptionListString::affect(const std::string &argument)
{
	_values.push_back(argument);
	affected() += 1;
}

const vector<string> &OptionListString::values() const
{
	return _values;
}
