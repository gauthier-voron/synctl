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


#include "synctl/io/FdOutputStream.hxx"

#include <unistd.h>

#include <cstdint>
#include <cstdlib>

#include "synctl/io/IOException.hxx"


using std::string;
using synctl::FdOutputStream;
using synctl::IOException;


FdOutputStream::FdOutputStream()
{
}

FdOutputStream::FdOutputStream(int fd)
	: _fd(fd)
{
}

FdOutputStream::FdOutputStream(FdOutputStream &&other)
	: _fd(other._fd)
{
	other._fd = -1;
}

FdOutputStream::~FdOutputStream()
{
	close();
}

FdOutputStream &FdOutputStream::operator=(FdOutputStream &&other)
{
	close();
	_fd = other._fd;
	other._fd = -1;
	return *this;
}

void FdOutputStream::write(uint8_t c)
{
	write(&c, sizeof (c));
}

void FdOutputStream::write(const uint8_t *src, size_t len)
{
	ssize_t ssize;

	ssize = ::write(_fd, src, len);
	if ((ssize < 0) || (((size_t) ssize) != len))
		throw IOException("cannot write properly");
}

void FdOutputStream::close()
{
	if (_fd >= 0)
		::close(_fd);
	_fd = -1;
}
