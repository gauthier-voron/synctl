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


#include "synctl/ui/OptionWithArgumentException.hxx"

#include <string>

#include "synctl/ui/OptionWithArgumentException.hxx"


using std::string;
using synctl::OptionException;
using synctl::OptionWithArgumentException;


OptionWithArgumentException::OptionWithArgumentException(const string &what)
	: OptionException(what)
{
}
