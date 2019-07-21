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


#ifndef _INCLUDE_SYNCTL_LINKTABLE_1_HXX_
#define _INCLUDE_SYNCTL_LINKTABLE_1_HXX_


#include <set>

#include "synctl/tree/Reference.hxx"


namespace synctl {


class InputStream;
class OutputStream;


class Linktable_1
{
	std::set<Reference>  _links;


	void _write(OutputStream *output) const;

	void _read(InputStream *input);


 public:
	Linktable_1() = default;
	Linktable_1(const Linktable_1 &other) = default;

	Linktable_1 &operator=(const Linktable_1 &other) = default;


	void addLink(const Reference &reference);

	void removeLink(const Reference &reference);

	const std::set<Reference> &getLinks() const;

	size_t size() const;


	void write(OutputStream *output, Reference *ref = nullptr) const;

	void read(InputStream *input, Reference *ref = nullptr);
};


}


#endif
