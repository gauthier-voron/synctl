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


#ifndef _INCLUDE_SYNCTL_REPOSITORY_HXX_
#define _INCLUDE_SYNCTL_REPOSITORY_HXX_


#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "synctl/repo/Branch.hxx"
#include "synctl/repo/BranchStore.hxx"
#include "synctl/repo/ObjectStore.hxx"
#include "synctl/repo/ReferenceStore.hxx"
#include "synctl/repo/TrunkStore.hxx"


namespace synctl {


class Trunk;
class InputStream;
class OutputStream;
class Reference;
class TransientOutputStream;


class Repository
{
	std::string     _path;
	BranchStore     _bstore;
	ObjectStore     _ostore;
	ReferenceStore  _rstore;
	TrunkStore      _tstore;


 public:
	Repository(const std::string &path);


	void initialize() const;


	void takeReference(const Reference &reference);

	void dumpReferences(OutputStream *output) const;


	std::unique_ptr<TransientOutputStream> newObject();

	std::unique_ptr<InputStream>
	readObject(const Reference &reference) const;

	size_t getObjectSize(const Reference &reference) const;


	Branch *setBranch(const std::string &name,
			  const Branch::Content &content);

	Branch *branch(const std::string &name);
	const Branch *branch(const std::string &name) const;

	std::map<std::string, Branch *> branches();
	std::map<std::string, const Branch *> branches() const;


	Trunk *newTrunk(const std::string &name);

	Trunk *trunk(const std::string &name);
	const Trunk *trunk(const std::string &name) const;

	std::map<std::string, Trunk *> trunks();
	std::map<std::string, const Trunk *> trunks() const;
};


}


#endif
