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


#ifndef _INCLUDE_SYNCTL_BRANCHSTORE_HXX_
#define _INCLUDE_SYNCTL_BRANCHSTORE_HXX_


#include <map>
#include <memory>
#include <string>

#include "synctl/io/Directory.hxx"
#include "synctl/repo/Branch.hxx"


namespace synctl {


class BranchStore
{
	Directory                                              _dir;
	mutable std::map<std::string, std::unique_ptr<Branch>> _branches;
	mutable std::map<std::string, Branch *>                _rwptrs;
	mutable std::map<std::string, const Branch *>          _roptrs;
	mutable bool                                           _loaded = false;


	void _ensureLoaded() const;


 public:
	BranchStore(const std::string &path);


	void initialize() const;
	void load() const;


	Branch *setBranch(const std::string &name,
			  const Branch::Content &content);

	Branch *branch(const std::string &name) noexcept;
	const Branch *branch(const std::string &name) const noexcept;

	std::map<std::string, Branch *> &branches() noexcept;
	std::map<std::string, const Branch *> &branches() const noexcept;
};


}


#endif
