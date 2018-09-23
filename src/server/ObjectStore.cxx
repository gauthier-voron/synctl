#include "synctl/server/ObjectStore.hxx"

#include <sys/stat.h>
#include <sys/types.h>

#include <cstdint>
#include <memory>
#include <string>

#include "synctl/FileInputStream.hxx"
#include "synctl/FileOutputStream.hxx"
#include "synctl/IOException.hxx"
#include "synctl/InputStream.hxx"
#include "synctl/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/server/TransientOutputStream.hxx"


using std::make_unique;
using std::string;
using std::unique_ptr;
using synctl::FileInputStream;
using synctl::FileOutputStream;
using synctl::IOException;
using synctl::InputStream;
using synctl::OutputStream;
using synctl::Reference;
using synctl::server::ObjectStore;
using synctl::server::TransientOutputStream;


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
	for (i = 2; i < ref.size(); i++)
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
	for (i = 2; i < ref.size(); i++)
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

unique_ptr<OutputStream> ObjectStore::_writeReferencePath(const Reference &ref)
{
	string path = _buildReferencePath(ref);
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
	Refcount refcnt;

	if (input != nullptr)
		input->readall(&refcnt, sizeof (refcnt));

	return input;
}

ObjectStore::Refcount ObjectStore::readRefcount(const Reference &reference)
	const
{
	unique_ptr<InputStream> input = _readReferencePath(reference);
	Refcount refcnt = 0;

	if (input != nullptr)
		input->readall(&refcnt, sizeof (refcnt));

	return refcnt;
}

void ObjectStore::writeRefcount(const Reference &reference, Refcount cnt)
{
	unique_ptr<OutputStream> output = _writeReferencePath(reference);

	if (output != nullptr)
		output->write(&cnt, sizeof (cnt));
}

void ObjectStore::takeReference(const Reference &reference)
{
	Refcount refcnt = readRefcount(reference);
	writeRefcount(reference, refcnt + 1);
}

unique_ptr<TransientOutputStream> ObjectStore::newObject()
{
	return make_unique<TransientOutputStream>(this);
}

unique_ptr<OutputStream> ObjectStore::newObject(const Reference &reference)
{
	unique_ptr<OutputStream> output = _writeReferencePath(reference);
	Refcount refcnt = 0;

	output->write(&refcnt, sizeof (refcnt));

	return output;
}
