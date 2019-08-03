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


#ifndef _INCLUDE_SYNCTL_DESCRIBE_1_HXX_
#define _INCLUDE_SYNCTL_DESCRIBE_1_HXX_


#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>

#include <string>


namespace synctl {


class OutputStream;
class Repository;


class Describe_1
{
	std::string  _trunkRegex = ".*";
	std::string  _branchRegex = "";

	mutable ::pcre2_code  *_trunkCode = nullptr;
	mutable ::pcre2_code  *_branchCode = nullptr;


	void _ensureCompiled() const;

	void _describeBranches(OutputStream *output,
			       const Repository *repository) const;

	void _describeTrunks(OutputStream *output,
			     const Repository *repository) const;


 public:
	~Describe_1();

	std::string &trunkRegex();

	std::string &branchRegex();

	void describe(OutputStream *output, const Repository *repository)
		const;
};


}


#endif
