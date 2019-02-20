#include "synctl/io/Symlink.hxx"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "synctl/io/IOException.hxx"


#define SYMLINK_PATH_BUFFER_SIZE  128


using std::string;
using synctl::IOException;
using synctl::Symlink;


Symlink::Symlink(const string &path)
	: _path(path)
{
}

bool Symlink::exists() const
{
	struct stat st;

	if (::lstat(_path.c_str(), &st) != 0)
		return false;

	return ((st.st_mode & S_IFMT) == S_IFLNK);
}

string Symlink::get() const
{
	ssize_t s, l = SYMLINK_PATH_BUFFER_SIZE;
	string target;

	target.resize(l);

	while ((s = ::readlink(_path.c_str(), target.data(), l)) == l) {
		l = l << 1;
		target.resize(l);
	}

	if (s < 0)
		throw IOException();

	target.resize(s);
	return target;
}

void Symlink::remove()
{
	int ret;

	ret = ::unlink(_path.c_str());
	if (ret != 0)
		throw IOException();
}

void Symlink::create(const string &target)
{
	int ret;

	ret = ::symlink(target.c_str(), _path.c_str());
	if (ret != 0)
		throw IOException();
}

void Symlink::set(const string &target)
{
	remove();
	create(target);
}
