#include "synctl/repo/TrunkStore.hxx"

#include <map>
#include <memory>
#include <string>

#include "synctl/io/IOException.hxx"
#include "synctl/repo/Trunk.hxx"


using std::make_unique;
using std::map;
using std::move;
using std::string;
using std::unique_ptr;
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

		_trunks[name] = move(trunk);
		_rwptrs[name] = ptr;
		_roptrs[name] = ptr;
	}

	_loaded = true;
}

Trunk *TrunkStore::newTrunk(const string &name)
{
	string path = _dir.path() + "/" + name;
	unique_ptr<Trunk> trunk = make_unique<Trunk>(path);
	Trunk *ptr = trunk.get();

	trunk->initialize();

	_ensureLoaded();

	_trunks[name] = move(trunk);
	_rwptrs[name] = ptr;
	_roptrs[name] = ptr;

	return ptr;
}

Trunk *TrunkStore::trunk(const string &name) noexcept
{
	_ensureLoaded();

	auto it = _rwptrs.find(name);

	if (it == _rwptrs.end())
		return nullptr;
	return it->second;
}

const Trunk *TrunkStore::trunk(const string &name) const noexcept
{
	_ensureLoaded();

	auto it = _roptrs.find(name);

	if (it == _roptrs.end())
		return nullptr;
	return it->second;
}

const map<string, Trunk *> &TrunkStore::trunks() noexcept
{
	_ensureLoaded();
	return _rwptrs;
}

const map<string, const Trunk *> &TrunkStore::trunks() const noexcept
{
	_ensureLoaded();
	return _roptrs;
}
