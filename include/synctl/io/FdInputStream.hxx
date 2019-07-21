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


#ifndef _INCLUDE_SYNCTL_FDINPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_FDINPUTSTREAM_HXX_


#include <cstdint>

#include "synctl/io/InputStream.hxx"


namespace synctl {


class FdInputStream : public InputStream
{
	int  _fd = -1;

	
 public:
	FdInputStream();
	FdInputStream(int fd);
	FdInputStream(const FdInputStream &other) = delete;
	FdInputStream(FdInputStream &&other);
	virtual ~FdInputStream();

	FdInputStream &operator=(const FdInputStream &other) = delete;
	FdInputStream &operator=(FdInputStream &&other);

	virtual int16_t read();
	virtual size_t read(uint8_t *dest, size_t len);
	using InputStream::read;

	using InputStream::readall;

	virtual void close();
};


}


#endif
