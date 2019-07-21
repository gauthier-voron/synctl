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


#include "synctl/repo/Repository.hxx"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "synctl/io/FileInputStream.hxx"
#include "synctl/io/FileOutputStream.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/IOException.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/repo/Branch.hxx"
#include "synctl/repo/BranchStore.hxx"
#include "synctl/repo/Trunk.hxx"
#include "synctl/repo/TrunkStore.hxx"
#include "synctl/io/TransientOutputStream.hxx"


using std::make_unique;
using std::map;
using std::string;
using std::unique_ptr;
using synctl::Branch;
using synctl::BranchStore;
using synctl::FileInputStream;
using synctl::FileOutputStream;
using synctl::InputStream;
using synctl::IOException;
using synctl::OutputStream;
using synctl::Reference;
using synctl::Repository;
using synctl::TransientOutputStream;
using synctl::Trunk;


static string __bstorePath(const string &path)
{
	string bstore = path + "/branches";
	return bstore;
}

static string __tstorePath(const string &path)
{
	string tstore = path + "/trunks";
	return tstore;
}

static string __ostorePath(const string &path)
{
	string ostore = path + "/objects";
	return ostore;
}

static string __rstorePath(const string &path)
{
	string ostore = path + "/references";
	return ostore;
}

Repository::Repository(const string &path)
	: _path(path), _bstore(__bstorePath(path))
	, _ostore(__ostorePath(path)), _rstore(__rstorePath(path))
	, _tstore(__tstorePath(path))
{
}

void Repository::initialize() const
{
	struct dirent *de;
	struct stat st;
	string ename;
	DIR *dd;

	if (stat(_path.c_str(), &st) != 0) {
		if (mkdir(_path.c_str(), 0700) != 0)
			throw IOException("cannot create directory");
	} else {
		if ((st.st_mode & S_IFMT) != S_IFDIR)
			throw IOException("not a directory");

		dd = opendir(_path.c_str());
		if (dd == NULL)
			throw IOException("cannot open directory");

		while ((de = readdir(dd)) != NULL) {
			ename = string(de->d_name);
			if ((ename == ".") || (ename == ".."))
				continue;
			closedir(dd);
			throw IOException("directory not empty");
		}

		closedir(dd);
	}

	_bstore.initialize();
	_tstore.initialize();
	_ostore.initialize();
	_rstore.initialize();
}

void Repository::takeReference(const Reference &reference)
{
	_rstore.takeReference(reference);
}

void Repository::dumpReferences(OutputStream *output) const
{
	_rstore.dumpReferences(output);
}

unique_ptr<TransientOutputStream> Repository::newObject()
{
	return _ostore.newObject();
}

unique_ptr<InputStream> Repository::readObject(const Reference &reference)
	const
{
	return _ostore.readObject(reference);
}

size_t Repository::getObjectSize(const Reference &reference) const
{
	return _ostore.getObjectSize(reference);
}

Branch *Repository::setBranch(const string &name, const Branch::Content &cont)
{
	return _bstore.setBranch(name, cont);
}

Branch *Repository::branch(const string &name)
{
	return _bstore.branch(name);
}

const Branch *Repository::branch(const string &name) const
{
	return _bstore.branch(name);
}

map<string, Branch *> Repository::branches()
{
	return _bstore.branches();
}

map<string, const Branch *> Repository::branches() const
{
	return _bstore.branches();
}

Trunk *Repository::newTrunk(const string &name)
{
	return _tstore.newTrunk(name);
}

Trunk *Repository::trunk(const string &name)
{
	return _tstore.trunk(name);
}

const Trunk *Repository::trunk(const string &name) const
{
	return _tstore.trunk(name);
}

map<string, Trunk *> Repository::trunks()
{
	return _tstore.trunks();
}

map<string, const Trunk *> Repository::trunks() const
{
	return _tstore.trunks();
}
