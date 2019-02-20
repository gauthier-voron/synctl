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
#include "synctl/io/TransientOutputStream.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/repo/OverwriteException.hxx"


using std::make_unique;
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

void ObjectStore::dumpReferences(OutputStream *output) const
{
	Directory root = Directory(_path);
	string path = _path;
	Directory stub;
	string hexref;
	Reference ref;

	path.push_back('/');

	for (const string &stubName : root.trueChildren()) {
		hexref = stubName;

		path.append(stubName);
		stub = Directory(path);

		for (const string &queue : stub.trueChildren()) {
			hexref.append(queue);
			ref = Reference::fromHex(hexref);
			output->write(ref.data(), ref.size());
			hexref.resize(2);
		}

		path.resize(path.length() - 2);
	}
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
	Refcount refcnt;

	if (input != nullptr)
		input->readall(&refcnt, sizeof (refcnt));

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

	return (st.st_size - sizeof (ObjectStore::Refcount));
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
	unique_ptr<OutputStream> output = _writeReferencePath(reference,false);

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
	unique_ptr<OutputStream> output = _writeReferencePath(reference, true);
	Refcount refcnt = 0;

	output->write(&refcnt, sizeof (refcnt));

	return output;
}
