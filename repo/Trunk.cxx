#include "synctl/repo/Trunk.hxx"

#include <map>
#include <memory>
#include <string>

#include "synctl/io/IOException.hxx"
#include "synctl/io/Path.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/repo/Snapshot.hxx"


#define SNAPSHOT_NAME_LENGTH   10


using std::make_unique;
using std::map;
using std::string;
using std::unique_ptr;
using synctl::IOException;
using synctl::Reference;
using synctl::Snapshot;
using synctl::Trunk;


void Trunk::_ensureLoaded() const
{
	if (_loaded == false)
		load();
}

Snapshot *Trunk::_newSnapshot(const string &name,
			      const Snapshot::Content &content) const
{
	string path = _dir.path() + "/" + name;
	unique_ptr<Snapshot> uptr = Snapshot::makePtr(path, content);
	Snapshot *ptr;

	_snapshots[name] = move(uptr);
	ptr = _snapshots[name].get();
	_rwptrs[name] = ptr;
	_roptrs[name] = ptr;

	return ptr;
}

Snapshot *Trunk::_newSnapshot(string *name, const Snapshot::Content &content)
	const
{
	string path = _dir.path() + "/";
	size_t len = path.length();

	_ensureLoaded();
	path = randomHexPath(path, SNAPSHOT_NAME_LENGTH);

	*name = path.substr(len);

	return _newSnapshot(*name, content);
}

Snapshot *Trunk::_snapshot(const string &name) const
{
	string path = _dir.path() + "/" + name;
	Snapshot *ptr;

	_snapshots[name] = make_unique<Snapshot>(path);
	ptr = _snapshots[name].get();
	_rwptrs[name] = ptr;
	_roptrs[name] = ptr;

	return ptr;
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
	for (const string &name : _dir.trueChildren())
		_snapshot(name);
	_loaded = true;
}

Snapshot *Trunk::newSnapshot(const Snapshot::Content &content, string *name)
{
	Snapshot *ret = _newSnapshot(name, content);
	return ret;
}

Snapshot *Trunk::snapshot(const string &name) noexcept
{
	_ensureLoaded();

	auto it = _rwptrs.find(name);

	if (it == _rwptrs.end())
		return nullptr;
	return it->second;
}

const Snapshot *Trunk::snapshot(const string &name) const noexcept
{
	_ensureLoaded();

	auto it = _roptrs.find(name);

	if (it == _roptrs.end())
		return nullptr;
	return it->second;
}

Snapshot *Trunk::lastSnapshot()
{
	Snapshot *last = nullptr;
	Snapshot::Date date, lastDate;

	for (const auto &pair : snapshots()) {
		date = pair.second->content().date;

		if ((last == nullptr) || (date > lastDate)) {
			last = pair.second;
			lastDate = date;
		}
	}

	return last;
}

const Snapshot *Trunk::lastSnapshot() const
{
	const Snapshot *last = nullptr;
	Snapshot::Date date, lastDate;

	for (const auto &pair : snapshots()) {
		date = pair.second->content().date;

		if ((last == nullptr) || (date > lastDate)) {
			last = pair.second;
			lastDate = date;
		}
	}

	return last;
}

const map<string, Snapshot *> &Trunk::snapshots() noexcept
{
	_ensureLoaded();
	return _rwptrs;
}

const map<string, const Snapshot *> &Trunk::snapshots() const noexcept
{
	_ensureLoaded();
	return _roptrs;
}

const string &Trunk::path() const noexcept
{
	return _dir.path();
}
