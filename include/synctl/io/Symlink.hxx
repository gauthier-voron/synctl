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


#ifndef _INCLUDE_SYNCTL_SYNLINK_HXX_
#define _INCLUDE_SYNCTL_SYNLINK_HXX_


namespace synctl {


class Symlink
{
	std::string  _path;


 public:
	Symlink() = default;
	Symlink(const std::string &path);
	Symlink(Symlink &&other) = default;

	Symlink &operator=(Symlink &&other) = default;


	bool exists() const;

	std::string get() const;


	void remove();

	void create(const std::string &target);

	void set(const std::string &target);
};


}


#endif
