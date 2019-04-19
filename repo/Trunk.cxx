#include "synctl/repo/Trunk.hxx"

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
using synctl::Snapshot;
using synctl::Trunk;


void Trunk::_ensureLoaded() const
{
	if (_loaded == false)
		load();
}

Snapshot *Trunk::_newSnapshot(const string &path) const
{
	Snapshot *ptr;

	_snapshots.emplace_back(make_unique<Snapshot>(path));
	ptr = _snapshots.back().get();
	_rwptrs.push_back(ptr);
	_roptrs.push_back(ptr);

	return ptr;
}

Snapshot *Trunk::_newSnapshot(string *name) const
{
	string path = _dir.path() + "/";
	size_t len = path.length();

	_ensureLoaded();
	path = randomHexPath(path, SNAPSHOT_NAME_LENGTH);

	*name = path.substr(len);

	return _newSnapshot(path);
}

Trunk::Trunk(const string &path)
	: _dir(path)
{
}

void Trunk::initialize() const
{
	_dir.create();
	_loaded = true;
}

void Trunk::load() const
{
	string path;

	for (const string &name : _dir.trueChildren()) {
		path = _dir.path() + "/" + name;
		_newSnapshot(path);
	}

	_loaded = true;
}

Snapshot *Trunk::newSnapshot(const Snapshot::Content &content, string *name)
{
	Snapshot *ret = _newSnapshot(name);
	ret->initialize(content);
	return ret;
}

Snapshot *Trunk::snapshot(const string &name) noexcept
{
	string path = _dir.path() + "/" + name;

	_ensureLoaded();

	for (Snapshot *sn : _rwptrs)
		if (sn->path() == path)
			return sn;

	return nullptr;
}

const Snapshot *Trunk::snapshot(const string &name) const noexcept
{
	string path = _dir.path() + "/" + name;

	_ensureLoaded();

	for (const Snapshot *sn : _roptrs)
		if (sn->path() == path)
			return sn;

	return nullptr;
}

const string &Trunk::path() const noexcept
{
	return _dir.path();
}

const vector<Snapshot *> &Trunk::snapshots() noexcept
{
	_ensureLoaded();
	return _rwptrs;
}

const vector<const Snapshot *> &Trunk::snapshots() const noexcept
{
	_ensureLoaded();
	return _roptrs;
}
