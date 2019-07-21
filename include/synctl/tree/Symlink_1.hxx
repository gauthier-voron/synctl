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


#ifndef _INCLUDE_SYNCTL_SYMLINK_1_HXX_
#define _INCLUDE_SYNCTL_SYMLINK_1_HXX_


#include <string>


namespace synctl {


class InputStream;
class OutputStream;
class Reference;


class Symlink_1
{
	std::string  _target;


	void _write(OutputStream *output) const;

	void _read(InputStream *input);


 public:
	Symlink_1() = default;
	explicit Symlink_1(const std::string &target);
	Symlink_1(const Symlink_1 &other) = default;

	Symlink_1 &operator=(const Symlink_1 &other) = default;


	void setTarget(const std::string &target);
	std::string getTarget() const;


	void write(OutputStream *output, Reference *ref = nullptr) const;

	void read(InputStream *input, Reference *ref = nullptr);


	static Symlink_1 make(const std::string &path);

	void apply(const std::string &path) const;
};


}


#endif
