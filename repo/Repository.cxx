#include "synctl/repo/Repository.hxx"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
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
#include "synctl/io/TransientOutputStream.hxx"


using std::make_unique;
using std::string;
using std::unique_ptr;
using synctl::FileInputStream;
using synctl::FileOutputStream;
using synctl::InputStream;
using synctl::IOException;
using synctl::OutputStream;
using synctl::Reference;
using synctl::Branch;
using synctl::Repository;
using synctl::TransientOutputStream;


// unique_ptr<InputStream> Repository::_readObject(const Reference &ref) const
// {
// 	string object_path = string(_path + "/objects/" + ref.toHex());
// 	return make_unique<FileInputStream>(object_path);
// }

// Repository::Refcount Repository::_readRefcount(const Reference &reference) const
// {
// 	unique_ptr<InputStream> is = _readObject(reference);
// 	Refcount ret;

// 	is->readall(&ret, sizeof (ret));

// 	return ret;
// }

// unique_ptr<OutputStream> Repository::_writeObject(const Reference &reference)
// {
// 	string object_path = string(_path + "/objects/" + reference.toHex());
// 	return make_unique<FileOutputStream>(object_path);
// }

// void Repository::_writeRefcount(const Reference &reference, Refcount cnt)
// {
// 	unique_ptr<OutputStream> os = _writeObject(reference);
// 	os->write(&cnt, sizeof (cnt));
// }

static string __bstorePath(const string &path)
{
	string bstore = path + "/branches";
	return bstore;
}

static string __ostorePath(const string &path)
{
	string ostore = path + "/objects";
	return ostore;
}

Repository::Repository(const string &path)
	: _path(path), _bstore(__bstorePath(path)), _ostore(__ostorePath(path))
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
	_ostore.initialize();
}

void Repository::takeReference(const Reference &reference)
{
	_ostore.takeReference(reference);
}

void Repository::dumpReferences(OutputStream *output) const
{
	_ostore.dumpReferences(output);
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

Branch *Repository::newBranch(const string &name)
{
	return _bstore.newBranch(name);
}

Branch *Repository::branch(const string &name)
{
	return _bstore.branch(name);
}

const Branch *Repository::branch(const string &name) const
{
	return _bstore.branch(name);
}
