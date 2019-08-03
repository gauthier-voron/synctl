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


#ifndef _INCLUDE_SYNCTL_PULL_1_HXX_
#define _INCLUDE_SYNCTL_PULL_1_HXX_


#include <set>
#include <string>

#include "synctl/io/LinkTracker.hxx"
#include "synctl/plan/Opcode.hxx"


namespace synctl {


class InputStream;


class Pull_1
{
	struct Context
	{
		std::string             apath;
		std::string             rpath;
		opcode_t                opcode;
		std::set<std::string>  *rdone;
		InputStream            *input;
		LinkTracker            *tracker;
	};


	void _delete(const std::string &path);
	void _delete(const Context *context);

	bool _linkLocally(const Context *context);

	void _pullObject(const Context *context);

	void _createDirectory(const Context *context);

	void _mergeDirectory(const Context *context);

	void _pullDirectory(const Context *context);

	void _pullRegular(const Context *context);

	void _pullSymlink(const Context *context);

	void _pullLinks(const Context *context);

	void _pullLink(const Context *context);


 public:
	void pull(InputStream *input, const std::string &root);
};


}


#endif
