#include "synctl/repo/Branch.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/IOException.hxx"
#include "synctl/io/Path.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/repo/Snapshot.hxx"


#define SNAPSHOT_NAME_LENGTH   10


using std::make_unique;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::IOException;
using synctl::Reference;
using synctl::Branch;
using synctl::Snapshot;


void Branch::_ensureLoaded() const
{
	if (_loaded == false)
		load();
}

Snapshot *Branch::_newSnapshot(const string &path) const
{
	Snapshot *ptr;

	_snapshots.emplace_back(make_unique<Snapshot>(path));
	ptr = _snapshots.back().get();
	_rwptrs.push_back(ptr);
	_roptrs.push_back(ptr);

	return ptr;
}

Snapshot *Branch::_newSnapshot(string *name) const
{
	string path = _dir.path() + "/";
	size_t len = path.length();

	_ensureLoaded();
	path = randomHexPath(path, SNAPSHOT_NAME_LENGTH);

	*name = path.substr(len);

	return _newSnapshot(path);
}

Branch::Branch(const string &path)
	: _dir(path)
{
}

void Branch::initialize() const
{
	_dir.create();
	_loaded = true;
}

void Branch::load() const
{
	string path;

	for (const string &name : _dir.trueChildren()) {
		path = _dir.path() + "/" + name;
		_newSnapshot(path);
	}

	_loaded = true;
}

Snapshot *Branch::newSnapshot(const Snapshot::Content &content, string *name)
{
	Snapshot *ret = _newSnapshot(name);
	ret->initialize(content);
	return ret;
}

Snapshot *Branch::snapshot(const string &name) noexcept
{
	string path = _dir.path() + "/" + name;

	_ensureLoaded();

	for (Snapshot *sn : _rwptrs)
		if (sn->path() == path)
			return sn;

	return nullptr;
}

const Snapshot *Branch::snapshot(const string &name) const noexcept
{
	string path = _dir.path() + "/" + name;

	_ensureLoaded();

	for (const Snapshot *sn : _roptrs)
		if (sn->path() == path)
			return sn;

	return nullptr;
}

const string &Branch::path() const noexcept
{
	return _dir.path();
}

const vector<Snapshot *> &Branch::snapshots() noexcept
{
	_ensureLoaded();
	return _rwptrs;
}

const vector<const Snapshot *> &Branch::snapshots() const noexcept
{
	_ensureLoaded();
	return _roptrs;
}
