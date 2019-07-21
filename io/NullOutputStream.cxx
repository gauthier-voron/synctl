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


#include "synctl/io/NullOutputStream.hxx"

#include <cstdint>


using synctl::NullOutputStream;


void NullOutputStream::write(uint8_t c [[gnu::unused]])
{
	_size += 1;
}

void NullOutputStream::write(const uint8_t *src [[gnu::unused]], size_t len)
{
	_size += len;
}

size_t NullOutputStream::written() const
{
	return _size;
}

void NullOutputStream::reset()
{
	_size = 0;
}
