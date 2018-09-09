#include "synctl/server/Repository.hxx"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <memory>
#include <string>

#include "synctl/FileInputStream.hxx"
#include "synctl/FileOutputStream.hxx"
#include "synctl/InputStream.hxx"
#include "synctl/IOException.hxx"
#include "synctl/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/server/TransientOutputStream.hxx"


using std::make_unique;
using std::string;
using std::unique_ptr;
using synctl::FileInputStream;
using synctl::FileOutputStream;
using synctl::InputStream;
using synctl::IOException;
using synctl::OutputStream;
using synctl::Reference;
using synctl::server::Repository;
using synctl::server::TransientOutputStream;


unique_ptr<InputStream> Repository::_readObject(const Reference &ref) const
{
	string object_path = string(_path + "/objects/" + ref.toHex());
	return make_unique<FileInputStream>(object_path);
}

Repository::Refcount Repository::_readRefcount(const Reference &reference) const
{
	unique_ptr<InputStream> is = _readObject(reference);
	Refcount ret;

	is->readall(&ret, sizeof (ret));

	return ret;
}

unique_ptr<OutputStream> Repository::_writeObject(const Reference &reference)
{
	string object_path = string(_path + "/objects/" + reference.toHex());
	return make_unique<FileOutputStream>(object_path);
}

void Repository::_writeRefcount(const Reference &reference, Refcount cnt)
{
	unique_ptr<OutputStream> os = _writeObject(reference);
	os->write(&cnt, sizeof (cnt));
}

Repository::Repository(const string &path)
	: _path(path)
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

	if (mkdir((_path + "/objects").c_str(), 0700) != 0)
		throw IOException("cannot create directory");
}

void Repository::takeReference(const Reference &reference)
{
	Refcount refcnt = _readRefcount(reference);
	_writeRefcount(reference, refcnt + 1);
}

unique_ptr<TransientOutputStream> Repository::newObject()
{
	return make_unique<TransientOutputStream>(this);
}

unique_ptr<OutputStream> Repository::newObject(const Reference &reference)
{
	unique_ptr<OutputStream> ret = _writeObject(reference);
	Refcount refcnt = 0;

	ret->write(&refcnt, sizeof (refcnt));

	return ret;
}
