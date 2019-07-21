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


#include "synctl/ui/OptionBoolean.hxx"

#include <string>

#include "synctl/ui/Option.hxx"
#include "synctl/ui/OptionAffectedTwiceException.hxx"
#include "synctl/ui/OptionWithoutArgumentException.hxx"


using std::string;
using synctl::Option;
using synctl::OptionAffectedTwiceException;
using synctl::OptionBoolean;
using synctl::OptionWithoutArgumentException;


OptionBoolean::OptionBoolean(const std::string &longName)
	: Option(longName)
{
}

OptionBoolean::OptionBoolean(char shortName)
	: Option(shortName)
{
}

OptionBoolean::OptionBoolean(const std::string &longName, char shortName)
	: Option(longName, shortName)
{
}

bool OptionBoolean::acceptArgument() const
{
	return false;
}

void OptionBoolean::affect()
{
	if (affected() > 0)
		throw OptionAffectedTwiceException();

	_value = true;
	affected() += 1;
}

void OptionBoolean::affect(const std::string &argument [[gnu::unused]])
{
	throw OptionWithoutArgumentException();
}

bool OptionBoolean::value() const
{
	return _value;
}
