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


#ifndef _INCLUDE_SYNCTL_FILTER_HXX_
#define _INCLUDE_SYNCTL_FILTER_HXX_


#include <string>


namespace synctl {


class InputStream;
class OutputStream;


class Filter
{
 public:
	enum Action {
		Ignore,                          // Do the same than for parent
		Accept,                                    // Include the entry
		Reject,                // Exclude the entry and ignore children
		Traverse    // Check children and include if at least one is in
	};

	virtual ~Filter() = default;

	virtual Action apply(const std::string &path) = 0;

	virtual void write(OutputStream *output) const = 0;

	virtual void read(InputStream *input) = 0;
};


}


#endif
