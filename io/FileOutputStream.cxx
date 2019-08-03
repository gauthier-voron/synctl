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


#include "synctl/io/FileOutputStream.hxx"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <string>

#include "synctl/io/FdOutputStream.hxx"
#include "synctl/io/IOException.hxx"


using std::move;
using std::string;
using synctl::FdOutputStream;
using synctl::FileOutputStream;
using synctl::IOException;


FileOutputStream::FileOutputStream()
	: FdOutputStream()
{
}

FileOutputStream::FileOutputStream(const string &path)
{
	int fd = open(path.c_str(), O_WRONLY | O_CREAT, 0600);

	if (fd == -1)
		throw IOException("cannot open file");

	FdOutputStream::operator=(FdOutputStream(fd));
}

FileOutputStream::FileOutputStream(const string &path, int flags)
{
	int fd = open(path.c_str(), O_WRONLY | O_CREAT | flags, 0600);

	if (fd == -1)
		throw IOException("cannot open file");

	FdOutputStream::operator=(FdOutputStream(fd));
}

FileOutputStream::FileOutputStream(FileOutputStream &&other)
	: FdOutputStream(move(other))
{
}

FileOutputStream &FileOutputStream::operator=(FileOutputStream &&other)
{
	FdOutputStream::operator=(move(other));
	return *this;
}
