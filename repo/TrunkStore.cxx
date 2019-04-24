#include "synctl/repo/TrunkStore.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/IOException.hxx"
#include "synctl/repo/Trunk.hxx"


using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::IOException;
using synctl::Trunk;
using synctl::TrunkStore;


void TrunkStore::_ensureLoaded() const
{
	if (_loaded == false)
		load();
}

TrunkStore::TrunkStore(const string &path)
	: _dir(path)
{
}

void TrunkStore::initialize() const
{
	_dir.create();
	_loaded = true;
}

void TrunkStore::load() const
{
	unique_ptr<Trunk> trunk;
	string path;
	Trunk *ptr;

	for (const string &name : _dir.trueChildren()) {
		path = _dir.path() + "/" + name;
		trunk = make_unique<Trunk>(path);

		ptr = trunk.get();
		_trunks.push_back(move(trunk));
		_rwptrs.push_back(ptr);
		_roptrs.push_back(ptr);
	}

	_loaded = true;
}

Trunk *TrunkStore::newTrunk(const string &name)
{
	string path = _dir.path() + "/" + name;
	unique_ptr<Trunk> trunk = make_unique<Trunk>(path);
	Trunk *ptr;

	trunk->initialize();

	_ensureLoaded();

	ptr = trunk.get();
	_trunks.push_back(move(trunk));
	_rwptrs.push_back(ptr);
	_roptrs.push_back(ptr);

	return ptr;
}

Trunk *TrunkStore::trunk(const string &name) noexcept
{
	string path = _dir.path() + "/" + name;

	_ensureLoaded();

	for (Trunk *tr : _rwptrs)
		if (tr->path() == path)
			return tr;

	return nullptr;
}

const Trunk *TrunkStore::trunk(const string &name) const noexcept
{
	string path = _dir.path() + "/" + name;

	_ensureLoaded();

	for (const Trunk *tr : _roptrs)
		if (tr->path() == path)
			return tr;

	return nullptr;
}

const vector<Trunk *> &TrunkStore::trunks() noexcept
{
	_ensureLoaded();
	return _rwptrs;
}

const vector<const Trunk *> &TrunkStore::trunks() const noexcept
{
	_ensureLoaded();
	return _roptrs;
}
