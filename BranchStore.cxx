#include "synctl/BranchStore.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/IOException.hxx"
#include "synctl/Branch.hxx"


using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::IOException;
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
		_branches.push_back(move(branch));
		_rwptrs.push_back(ptr);
		_roptrs.push_back(ptr);
	}

	_loaded = true;
}

Branch *BranchStore::newBranch(const string &name)
{
	string path = _dir.path() + "/" + name;
	unique_ptr<Branch> branch = make_unique<Branch>(path);
	Branch *ptr;

	branch->initialize();

	_ensureLoaded();

	ptr = branch.get();
	_branches.push_back(move(branch));
	_rwptrs.push_back(ptr);
	_roptrs.push_back(ptr);

	return ptr;
}

Branch *BranchStore::branch(const string &name) noexcept
{
	string path = _dir.path() + "/" + name;

	_ensureLoaded();

	for (Branch *br : _rwptrs)
		if (br->path() == path)
			return br;

	return nullptr;
}

const Branch *BranchStore::branch(const string &name) const noexcept
{
	string path = _dir.path() + "/" + name;

	_ensureLoaded();

	for (const Branch *br : _roptrs)
		if (br->path() == path)
			return br;

	return nullptr;
}

const vector<Branch *> &BranchStore::branches() noexcept
{
	_ensureLoaded();
	return _rwptrs;
}

const vector<const Branch *> &BranchStore::branches() const noexcept
{
	_ensureLoaded();
	return _roptrs;
}
