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
