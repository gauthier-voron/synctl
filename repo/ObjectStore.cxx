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


#include "synctl/repo/ObjectStore.hxx"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <memory>
#include <string>

#include "synctl/io/Directory.hxx"
#include "synctl/io/FileInputStream.hxx"
#include "synctl/io/FileOutputStream.hxx"
#include "synctl/io/IOException.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/io/Path.hxx"
#include "synctl/io/TransientOutputStream.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/repo/OverwriteException.hxx"


#define TRANSIENT_NAME_LENGTH   20


using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using synctl::Directory;
using synctl::FileInputStream;
using synctl::FileOutputStream;
using synctl::IOException;
using synctl::InputStream;
using synctl::ObjectStore;
using synctl::OutputStream;
using synctl::OverwriteException;
using synctl::Refcount;
using synctl::Reference;
using synctl::TransientOutputStream;


bool ObjectStore::_getReferencePath(const Reference &ref, string *dest) const
{
	string hexref = ref.toHex();
	struct stat st;
	size_t i;
	int ret;

	*dest = _path;
	dest->push_back('/');
	dest->push_back(hexref[0]);
	dest->push_back(hexref[1]);
	dest->push_back('/');
	for (i = 2; i < hexref.size(); i++)
		dest->push_back(hexref[i]);

	ret = stat(dest->c_str(), &st);
	if ((ret != 0) || ((st.st_mode & S_IFMT) != S_IFREG))
		return false;

	return true;
}

string ObjectStore::_buildReferencePath(const Reference &ref)
{
	string hexref = ref.toHex();
	string path = _path;
	struct stat st;
	size_t i;
	int ret;

	path.push_back('/');
	path.push_back(hexref[0]);
	path.push_back(hexref[1]);

	ret = stat(path.c_str(), &st);
	if ((ret != 0) && (mkdir(path.c_str(), 0700) != 0))
		throw IOException("cannot create directory");

	path.push_back('/');
	for (i = 2; i < hexref.size(); i++)
		path.push_back(hexref[i]);

	return path;
}

unique_ptr<InputStream> ObjectStore::_readReferencePath(const Reference &ref)
	const
{
	string path;
	unique_ptr<InputStream> input;

	if (_getReferencePath(ref, &path) == false)
		return nullptr;
	return make_unique<FileInputStream>(path);
}

unique_ptr<OutputStream> ObjectStore::_writeReferencePath(const Reference &ref,
							  bool exclusive)
{
	string path = _buildReferencePath(ref);
	struct stat st;

	if (exclusive && (::stat(path.c_str(), &st) == 0))
		throw OverwriteException();

	return make_unique<FileOutputStream>(path);
}

ObjectStore::ObjectStore(const string &path)
	: _path(path)
{
}

void ObjectStore::initialize() const
{
	int ret = mkdir(_path.c_str(), 0700);

	if (ret != 0)
		throw IOException("cannot create directory");
}

unique_ptr<InputStream> ObjectStore::readObject(const Reference &ref) const
{
	unique_ptr<InputStream> input = _readReferencePath(ref);

	return input;
}

size_t ObjectStore::getObjectSize(const Reference &reference) const
{
	struct stat st;
	string path;

	if (_getReferencePath(reference, &path) == false)
		return 0;

	if (::stat(path.c_str(), &st) != 0)
		return 0;

	return st.st_size;
}

unique_ptr<TransientOutputStream> ObjectStore::newObject()
{
	string path = _path + "/transient-";
	FileOutputStream out = randomHexOutput(&path, TRANSIENT_NAME_LENGTH);

	return make_unique<TransientOutputStream>(this, path, move(out));
}

unique_ptr<OutputStream> ObjectStore::newObject(const Reference &reference)
{
	unique_ptr<OutputStream> output = _writeReferencePath(reference, true);

	return output;
}

void ObjectStore::putObject(const string &path, const Reference &reference)
{
	string dest = _buildReferencePath(reference);
	struct stat st;
	int ret;

	if (::stat(dest.c_str(), &st) == 0)
		throw OverwriteException();

	ret = ::rename(path.c_str(), dest.c_str());
	if (ret != 0)
		throw IOException();
}
