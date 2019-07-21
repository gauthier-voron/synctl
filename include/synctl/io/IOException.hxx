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


#ifndef _INCLUDE_SYNCTL_IOEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_IOEXCEPTION_HXX_


#include <string>


namespace synctl {


class IOException
{
	std::string  _what;


 public:
	IOException();
	IOException(const std::string &what);
	IOException(const IOException &other) = default;
	IOException(IOException &&other) = default;
	virtual ~IOException();

	IOException &operator=(const IOException &other) = default;
	IOException &operator=(IOException &&other) = default;

	std::string what() const noexcept;
};


}


#endif
