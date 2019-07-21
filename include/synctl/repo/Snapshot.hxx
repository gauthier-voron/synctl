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


#ifndef _INCLUDE_SYNCTL_SNAPSHOT_HXX_
#define _INCLUDE_SYNCTL_SNAPSHOT_HXX_


#include <cstdint>
#include <memory>
#include <string>

#include "synctl/io/Directory.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class Snapshot
{
 public:
	using Date = uint64_t;

	struct Content
	{
		Date       date = 0;  // UNIX time of creation (0 = now)
		opcode_t   opcode;    // Opcode of the object pointed by tree
		Reference  tree;      // Root of the snapshot
		Reference  links;     // Pointer to the LinkTable_1 object
	};


 private:
	const std::string  _path;
	mutable Content    _content;
	mutable bool       _loaded = false;


	Snapshot(const std::string &path, const Snapshot::Content &content);

	void _load() const;
	void _store() const;


 public:
	Snapshot(const std::string &path);

	const Content &content() const;
	const std::string &path() const noexcept;

	static Snapshot make(const std::string &path,
			     const Snapshot::Content &content);

	static std::unique_ptr<Snapshot>
	makePtr(const std::string &path, const Snapshot::Content &content);
};


}


#endif
