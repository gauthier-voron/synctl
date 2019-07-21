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


#include "synctl/ui/Option.hxx"


using std::string;
using synctl::Option;


Option::Option(const std::string &longName)
	: _longName(longName)
{
}

Option::Option(char shortName)
	: _shortName(shortName)
{
}

Option::Option(const std::string &longName, char shortName)
	: _longName(longName), _shortName(shortName)
{
}

const string &Option::longName() const
{
	return _longName;
}

char Option::shortName() const
{
	return _shortName;
}

void Option::affect()
{
	_affected += 1;
}

size_t &Option::affected()
{
	return _affected;
}

const size_t &Option::affected() const
{
	return _affected;
}
