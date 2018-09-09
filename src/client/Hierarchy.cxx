#include "synctl/client/Hierarchy.hxx"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "synctl/Regular.hxx"


using std::string;
using synctl::Directory;
using synctl::Regular;
using synctl::Symlink;
using synctl::client::Hierarchy;


Hierarchy::Hierarchy(const string &name, const string &root)
	: _name(name), _root(root)
{
}


#include <iostream>

using std::cout;
using std::endl;

void Hierarchy::_list_directory(const string &path) const
{
	DIR *dd = opendir(path.c_str());
	string ename, epath;
	struct dirent *de;

	if (dd == NULL)
		throw 0;

	while ((de = readdir(dd)) != NULL) {
		ename = string(de->d_name);
		if ((ename == ".") || (ename == ".."))
			continue;
		
		epath = path + '/' + ename;
		_list_entry(epath);
	}
	
	cout << "d: " << path << endl;
}

void Hierarchy::_list_regular(const string &path) const
{
	cout << "r: " << path << endl;
}

void Hierarchy::_list_symlink(const string &path) const
{
	cout << "l: " << path << endl;
}

void Hierarchy::_list_entry(const string &path) const
{
	struct stat statbuf;
	int ret;

	ret = lstat(path.c_str(), &statbuf);
	if (ret != 0)
		throw 0;

	switch (statbuf.st_mode & S_IFMT) {
	case S_IFDIR:
		_list_directory(path);
		return;
	case S_IFLNK:
		_list_symlink(path);
		return;
	case S_IFREG:
		_list_regular(path);
		return;
	default:
		throw 0;
	}
}

void Hierarchy::list() const
{
	_list_entry(_root);
}
