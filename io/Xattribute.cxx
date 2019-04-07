#include "synctl/io/Xattribute.hxx"

#include <sys/types.h>
#include <sys/xattr.h>

#include <map>
#include <string>

#include "synctl/io/IOException.hxx"


using std::map;
using std::string;
using synctl::IOException;


string synctl::getXattribute(const string &path, const string &name)
{
	const char *cpath = path.c_str();
	const char *cname = name.c_str();
	ssize_t ssize;
	string value;

	ssize = ::lgetxattr(cpath, cname, nullptr, 0);
	if (ssize < 0)
		throw IOException();

	value.resize((size_t) ssize);

	ssize = ::lgetxattr(cpath, cname, value.data(), value.size());
	if (ssize < 0)
		throw IOException();

	return value;
}

void synctl::setXattribute(const string &path, const string &name,
			   const string &value)
{
	int ret;

	ret = lsetxattr(path.c_str(), name.c_str(), value.data(),
			value.size(), 0);

	if (ret != 0)
		throw IOException();
}

void synctl::getXattributes(const string &path, map<string, string> *dest)
{
	string buffer, name, value;
	ssize_t ssize;
	size_t off;

	ssize = ::llistxattr(path.c_str(), nullptr, 0);

	if (ssize < 0)
		throw IOException();
	else if (ssize == 0)
		return;

	buffer.resize((size_t) ssize);

	ssize = ::llistxattr(path.c_str(), buffer.data(), buffer.size());
	if (ssize < 0)
		throw IOException();

	for (off = 0; off < buffer.size(); off += name.size() + 1) {
		name = string(buffer.c_str() + off);
		value = getXattribute(path, name);
		dest->emplace(name, value);
	}
}

map<string, string> synctl::getXattributes(const string &path)
{
	map<string, string> ret;

	getXattributes(path, &ret);
	return ret;
}

void synctl::setXattributes(const string &path,
			    const map<string, string> &attrs)
{
	for (auto &a : attrs)
		setXattribute(path, a.first, a.second);
}
