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


#include "synctl/ui/ProfileSyntaxException.hxx"

#include <string>

#include "synctl/ui/ConfigurationException.hxx"


using std::string;
using synctl::ConfigurationException;
using synctl::ProfileSyntaxException;


ProfileSyntaxException::ProfileSyntaxException(const string &lineText,
					       size_t line, size_t column,
					       const string &what)
	: ConfigurationException(what), _lineText(lineText), _line(line)
	, _column(column)
{
}

const string &ProfileSyntaxException::lineText() const noexcept
{
	return _lineText;
}

size_t ProfileSyntaxException::line() const noexcept
{
	return _line;
}

size_t ProfileSyntaxException::column() const noexcept
{
	return _column;
}
