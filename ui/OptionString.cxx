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


#include "synctl/ui/OptionString.hxx"

#include <string>

#include "synctl/ui/Option.hxx"
#include "synctl/ui/OptionAffectedTwiceException.hxx"
#include "synctl/ui/OptionWithArgumentException.hxx"


using std::string;
using synctl::Option;
using synctl::OptionAffectedTwiceException;
using synctl::OptionString;
using synctl::OptionWithArgumentException;


OptionString::OptionString(const string &longName, const string &defaultValue)
	: Option(longName), _value(defaultValue)
{
}

OptionString::OptionString(char shortName, const string &defaultValue)
	: Option(shortName), _value(defaultValue)
{
}

OptionString::OptionString(const string &longName, char shortName,
			   const string &defaultValue)
	: Option(longName, shortName), _value(defaultValue)
{
}

bool OptionString::acceptArgument() const
{
	return true;
}

void OptionString::affect()
{
	throw OptionWithArgumentException();
}

void OptionString::affect(const std::string &argument)
{
	if (affected() > 0)
		throw OptionAffectedTwiceException();

	_value = argument;
	affected() += 1;
}

const string &OptionString::value() const
{
	return _value;
}
