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


#ifndef _INCLUDE_SYNCTL_FDOUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_FDOUTPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>
#include <string>

#include "synctl/io/OutputStream.hxx"


namespace synctl {


class FdOutputStream : public OutputStream
{
	int  _fd = -1;


 public:
	FdOutputStream();
	FdOutputStream(int fd);
	FdOutputStream(const FdOutputStream &other) = delete;
	FdOutputStream(FdOutputStream &&other);
	virtual ~FdOutputStream();

	FdOutputStream &operator=(const FdOutputStream &other) = delete;
	FdOutputStream &operator=(FdOutputStream &&other);

	virtual void write(uint8_t c) override;
	virtual void write(const uint8_t *src, size_t len) override;
	using OutputStream::write;

	using OutputStream::flush;

	virtual void close() override;
};


}


#endif
