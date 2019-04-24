#include "synctl/repo/BranchStore.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/Directory.hxx"
#include "synctl/repo/Branch.hxx"


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
		_branches.push_back(move(branch));
		_rwptrs.push_back(ptr);
		_roptrs.push_back(ptr);
	}

	_loaded = true;
}

Branch *BranchStore::setBranch(const string &name, const Branch::Content &cont)
{
	string path = _dir.path() + "/" + name;
	Branch *ptr = nullptr;

	_ensureLoaded();

	for (Branch *br : _rwptrs)
		if (br->path() == path) {
			ptr = br;
			break;
		}

	if (ptr == nullptr) {
		_branches.push_back(make_unique<Branch>(path));
		ptr = _branches.back().get();
		_rwptrs.push_back(ptr);
		_roptrs.push_back(ptr);
	}

	ptr->store(cont);

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
