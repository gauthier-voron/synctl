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


#include "synctl/io/OutputStream.hxx"

#include <cstdlib>
#include <string>


using std::string;
using synctl::OutputStream;


OutputStream::~OutputStream()
{
	close();
}

void OutputStream::write(const uint8_t *src, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++)
		write(src[i]);
}

void OutputStream::write(const string &str)
{
	write(str.c_str(), str.length());
}

void OutputStream::writeStr(const string &str)
{
	write(str.c_str(), str.length() + 1);
}

void OutputStream::flush()
{
}

void OutputStream::close()
{
}
