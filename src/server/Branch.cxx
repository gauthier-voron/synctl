#include "synctl/server/Branch.hxx"

#include <sys/stat.h>
#include <sys/types.h>

#include <memory>
#include <random>
#include <string>
#include <vector>

#include "synctl/IOException.hxx"
#include "synctl/Reference.hxx"
#include "synctl/server/Snapshot.hxx"


#define SNAPSHOT_NAME_LENGTH   10


using std::make_unique;
using std::mt19937;
using std::random_device;
using std::string;
using std::uniform_int_distribution;
using std::unique_ptr;
using std::vector;
using synctl::IOException;
using synctl::Reference;
using synctl::server::Branch;
using synctl::server::Snapshot;


void Branch::_ensureLoaded() const
{
	if (_loaded == false)
		load();
}

string Branch::_newSnapshotPath() const
{
	random_device rd;
	mt19937 gen = mt19937(rd());
	uniform_int_distribution<uint64_t> dis;
	uint64_t number, digit;
	string numstr, path;
	struct stat st;
	size_t i;
	int ret;

	do {
		numstr.clear();
		number = dis(gen);
		for (i = 0; i < SNAPSHOT_NAME_LENGTH; i++) {
			digit = number & 0xf;
			if (digit < 10)
				digit = digit + '0';
			else
				digit = digit - 10 + 'a';
			numstr.push_back(static_cast<char> (digit));
			number >>= 4;
		}
		
		path = _dir.path() + "/" + numstr;
		ret = stat(path.c_str(), &st);
	} while (ret == 0);

	return path;
}

Snapshot *Branch::_newSnapshot(const string &path) const
{
	Snapshot *ptr;

	_ensureLoaded();

	_snapshots.emplace_back(make_unique<Snapshot>(path));
	ptr = _snapshots.back().get();
	_rwptrs.push_back(ptr);
	_roptrs.push_back(ptr);

	return ptr;
}

Snapshot *Branch::_newSnapshot() const
{
	string path = _newSnapshotPath();
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

Snapshot *Branch::newSnapshot(const Reference &ref)
{
	Snapshot *ret = _newSnapshot();
	ret->initialize(ref);
	return ret;
}

Snapshot *Branch::newSnapshot(Snapshot::Date d, const Reference &ref)
{
	Snapshot *ret = _newSnapshot();
	ret->initialize(d, ref);
	return ret;
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
