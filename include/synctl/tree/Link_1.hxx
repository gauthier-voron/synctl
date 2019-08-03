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


#ifndef _INCLUDE_SYNCTL_LINK_1_HXX_
#define _INCLUDE_SYNCTL_LINK_1_HXX_


#include <set>
#include <string>


namespace synctl {


class InputStream;
class OutputStream;
class Reference;


class Link_1
{
	std::set<std::string>  _locations;


	void _write(OutputStream *output) const;

	void _read(InputStream *input);


 public:
	Link_1() = default;
	Link_1(const Link_1 &other) = default;

	Link_1 &operator=(const Link_1 &other) = default;


	void addLocation(const std::string &name);

	void removeLocation(const std::string &name);

	const std::set<std::string> &getLocations() const;

	void clear();


	void write(OutputStream *output, Reference *ref = nullptr) const;

	void read(InputStream *input, Reference *ref = nullptr);
};


}


#endif
