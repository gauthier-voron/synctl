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


#include "synctl/repo/BranchStore.hxx"

#include <map>
#include <memory>
#include <string>

#include "synctl/io/Directory.hxx"
#include "synctl/repo/Branch.hxx"


using std::map;
using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using synctl::Branch;
using synctl::BranchStore;


void BranchStore::_ensureLoaded() const
{
	if (_loaded == false)
		load();
}

BranchStore::BranchStore(const string &path)
	: _dir(path)
{
}

void BranchStore::initialize() const
{
	_dir.create();
	_loaded = true;
}

void BranchStore::load() const
{
	unique_ptr<Branch> branch;
	string path;
	Branch *ptr;

	for (const string &name : _dir.trueChildren()) {
		path = _dir.path() + "/" + name;
		branch = make_unique<Branch>(path);

		ptr = branch.get();
		_branches[name] = move(branch);
		_rwptrs[name] = ptr;
		_roptrs[name] = ptr;
	}

	_loaded = true;
}

Branch *BranchStore::setBranch(const string &name, const Branch::Content &cont)
{
	unique_ptr<Branch> branch;
	Branch *ptr = nullptr;
	string path;

	_ensureLoaded();

	auto it = _rwptrs.find(name);

	if (it == _rwptrs.end()) {
		path = _dir.path() + "/" + name;

		branch = make_unique<Branch>(path);
		ptr = branch.get();

		_branches[name] = move(branch);
		_rwptrs[name] = ptr;
		_roptrs[name] = ptr;
	} else {
		ptr = it->second;
	}

	ptr->store(cont);

	return ptr;
}

Branch *BranchStore::branch(const string &name) noexcept
{
	_ensureLoaded();

	auto it = _rwptrs.find(name);

	if (it == _rwptrs.end())
		return nullptr;
	return it->second;
}

const Branch *BranchStore::branch(const string &name) const noexcept
{
	_ensureLoaded();

	auto it = _roptrs.find(name);

	if (it == _roptrs.end())
		return nullptr;
	return it->second;
}

map<string, Branch *> &BranchStore::branches() noexcept
{
	_ensureLoaded();
	return _rwptrs;
}

map<string, const Branch *> &BranchStore::branches() const noexcept
{
	_ensureLoaded();
	return _roptrs;
}
