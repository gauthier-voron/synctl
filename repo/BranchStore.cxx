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
