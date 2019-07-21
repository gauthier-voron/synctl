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


#ifndef _INCLUDE_SYNCTL_LIST_1_HXX_
#define _INCLUDE_SYNCTL_LIST_1_HXX_


#include <ctime>
#include <functional>
#include <string>
#include <vector>


namespace synctl {


class InputStream;


class List_1
{
 public:
	using Handler = std::function
		<void (const std::string &trunk,
		       const std::string &snapshot,
		       const struct timespec &date,
		       const std::vector<std::string> &branches)>;


 private:
	void _listBranch(InputStream *input, Handler handler);

	void _listTrunk(InputStream *input, Handler handler);


 public:
	void list(InputStream *input, Handler handler);
};


}


#endif
