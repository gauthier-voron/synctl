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


#include "synctl/io/InputStream.hxx"

#include <cstdint>
#include <cstdlib>

#include "synctl/io/EOFException.hxx"
#include "synctl/io/IOException.hxx"


using std::string;
using synctl::EOFException;
using synctl::InputStream;
using synctl::IOException;


InputStream::~InputStream()
{
	close();
}

size_t InputStream::read(uint8_t *dest, size_t len)
{
	size_t i;
	int16_t buf;

	for (i = 0; i < len; i++) {
		buf = read();
		if (buf == -1)
			throw IOException();
		dest[i] = static_cast<uint8_t> (buf);
	}

	return len;
}

void InputStream::readall(uint8_t *dest, size_t len)
{
	size_t s, done = 0;

	while (done < len) {
		s = read(dest + done, len - done);
		if (s == 0)
			throw EOFException();
		done += s;
	}
}

string InputStream::readStr()
{
	string ret;
	readStr(&ret);
	return ret;
}

void InputStream::readStr(string *dest)
{
	int16_t buf;

	dest->clear();

	while ((buf = read()) > 0)
		dest->push_back(static_cast<char> (buf));

	if (buf != 0)
		throw EOFException();
}

void InputStream::close()
{
}
