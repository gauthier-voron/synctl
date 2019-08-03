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


#include "synctl/io/FdInputStream.hxx"

#include <unistd.h>

#include "synctl/io/EOFException.hxx"
#include "synctl/io/IOException.hxx"


using synctl::EOFException;
using synctl::FdInputStream;
using synctl::IOException;


FdInputStream::FdInputStream()
{
}

FdInputStream::FdInputStream(int fd)
	: _fd(fd)
{
}

FdInputStream::FdInputStream(FdInputStream &&other)
	: _fd(other._fd)
{
	other._fd = -1;
}

FdInputStream::~FdInputStream()
{
	close();
}

FdInputStream &FdInputStream::operator=(FdInputStream &&other)
{
	close();
	_fd = other._fd;
	other._fd = -1;
	return *this;
}

int16_t FdInputStream::read()
{
	uint8_t val;
	size_t done = read(&val, sizeof (val));

	if (done == 0)
		return -1;
	return val;
}

size_t FdInputStream::read(uint8_t *dest, size_t len)
{
	ssize_t ssize = ::read(_fd, dest, len);

	if (ssize < 0)
		throw IOException("cannot read properly");

	return ((size_t) ssize);
}

void FdInputStream::close()
{
	if (_fd >= 0)
		::close(_fd);
	_fd = -1;
}
