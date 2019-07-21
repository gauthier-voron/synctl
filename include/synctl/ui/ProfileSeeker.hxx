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


#ifndef _INCLUDE_SYNCTL_PROFILESEEKER_HXX_
#define _INCLUDE_SYNCTL_PROFILESEEKER_HXX_


#include <string>
#include <vector>


namespace synctl {


class ProfileSeeker
{
	std::vector<std::string>  _paths;


	void _sanitize(std::string *path) const;


 public:
	ProfileSeeker() = default;
	explicit ProfileSeeker(const std::vector<std::string> &paths);
	ProfileSeeker(const ProfileSeeker &other) = default;
	ProfileSeeker(ProfileSeeker &&other) = default;

	ProfileSeeker &operator=(const ProfileSeeker &other) = default;
	ProfileSeeker &operator=(ProfileSeeker &&other) = default;

	void push(const std::string &paths);
	void push(const std::vector<std::string> &paths);

	void clear();

	bool seek(const std::string &name, std::string *dest) const;
	std::string seek(const std::string &name) const;

	const std::vector<std::string> &paths() const;
};


}


#endif
