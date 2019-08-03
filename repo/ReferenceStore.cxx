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


#include "synctl/repo/ReferenceStore.hxx"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <map>
#include <string>

#include "synctl/io/Directory.hxx"
#include "synctl/io/EOFException.hxx"
#include "synctl/io/FileInputStream.hxx"
#include "synctl/io/FileOutputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Reference.hxx"


using std::map;
using std::string;
using synctl::EOFException;
using synctl::FileInputStream;
using synctl::FileOutputStream;
using synctl::OutputStream;
using synctl::Refcount;
using synctl::Reference;
using synctl::ReferenceStore;


void ReferenceStore::_load() const
{
	static string path = _dir.path() + "/base";
	FileInputStream fis;
	Reference refbuf;
	Refcount cntbuf;
	struct stat st;
	int ret;

	ret = stat(path.c_str(), &st);
	if ((ret != 0) || ((st.st_mode & S_IFMT) != S_IFREG)) {
		_loaded = true;
		return;
	}

	fis = FileInputStream(path);

	_refcounts.clear();

	try {
		while (true) {
			fis.readall(refbuf.data(), refbuf.size());
			cntbuf = fis.readInt<Refcount>();
			_refcounts[refbuf] = cntbuf;
		}
	} catch (EOFException &) {
		// break
	}

	_loaded = true;
}

void ReferenceStore::_store() const
{
	static string path = _dir.path() + "/base";
	FileOutputStream fos = FileOutputStream(path);

	for (const auto &pair : _refcounts) {
		fos.write(pair.first.data(), pair.first.size());
		fos.writeInt(pair.second);
	}

	_loaded = true;
}

void ReferenceStore::_ensureLoaded() const
{
	if (_loaded == false)
		_load();
}

ReferenceStore::ReferenceStore(const string &path)
	: _dir(path)
{
}

ReferenceStore::~ReferenceStore()
{
	if (_loaded)
		_store();
}

void ReferenceStore::initialize() const
{
	_dir.create();
}

Refcount ReferenceStore::readRefcount(const Reference &reference) const
{
	_ensureLoaded();

	auto it = _refcounts.find(reference);

	if (it == _refcounts.end())
		return 0;

	return it->second;
}

void ReferenceStore::writeRefcount(const Reference &reference, Refcount cnt)
{
	_ensureLoaded();

	if (cnt == 0) {
		auto it = _refcounts.find(reference);

		if (it == _refcounts.end())
			return;

		_refcounts.erase(it);
	} else {
		_refcounts[reference] = cnt;
	}
}

void ReferenceStore::takeReference(const Reference &reference)
{
	Refcount old = readRefcount(reference);

	writeRefcount(reference, old + 1);
}

void ReferenceStore::dumpReferences(OutputStream *output) const
{
	_ensureLoaded();

	for (const auto &pair : _refcounts)
		output->write(pair.first.data(), pair.first.size());
}
