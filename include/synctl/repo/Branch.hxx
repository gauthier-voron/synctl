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


#ifndef _INCLUDE_SYNCTL_BRANCH_HXX_
#define _INCLUDE_SYNCTL_BRANCH_HXX_


#include <string>


namespace synctl {


class Branch
{
 public:
	struct Content
	{
		std::string  trunk;
		std::string  snapshot;
	};


 private:
	const std::string  _path;
	mutable Content    _content;
	mutable bool       _loaded = false;


 public:
	Branch(const std::string &path);

	void store(const Content &content);
	void load() const;

	const Content &content() const;
	const std::string &path() const noexcept;
};


}


#endif
