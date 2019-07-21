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


#ifndef _INCLUDE_SYNCTL_REFERENCE_HXX_
#define _INCLUDE_SYNCTL_REFERENCE_HXX_


#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <string>


namespace synctl {


class Reference
{
	uint8_t  _array[20];


 public:
	Reference();
	Reference(const Reference &other);


	Reference &operator=(const Reference &other);


	bool operator==(const Reference &other) const;
	bool operator!=(const Reference &other) const;
	bool operator<(const Reference &other) const;


	const uint8_t *data() const noexcept;
	uint8_t *data() noexcept;
	static size_t size() noexcept;


	static Reference fromHex(const std::string &hex);
	static const Reference &zero();
	std::string toHex() const;
};


}


#endif
