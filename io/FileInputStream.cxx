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


#include "synctl/io/FileInputStream.hxx"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <string>

#include "synctl/io/EOFException.hxx"
#include "synctl/io/IOException.hxx"


using std::move;
using std::string;
using synctl::FileInputStream;
using synctl::EOFException;
using synctl::IOException;


FileInputStream::FileInputStream()
	: FdInputStream()
{
}

FileInputStream::FileInputStream(const string path)
{
	int fd = open(path.c_str(), O_RDONLY);

	if (fd == -1)
		throw IOException("cannot open file");

	FdInputStream::operator=(FdInputStream(fd));
}

FileInputStream::FileInputStream(FileInputStream &&other)
	: FdInputStream(move(other))
{
}

FileInputStream &FileInputStream::operator=(FileInputStream &&other)
{
	FdInputStream::operator=(move(other));
	return *this;
}
