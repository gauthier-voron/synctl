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


#ifndef _INCLUDE_SYNCTL_NULLOUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_NULLOUTPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>

#include "synctl/io/OutputStream.hxx"


namespace synctl {


class NullOutputStream : public OutputStream
{
	size_t  _size = 0;


public:
	NullOutputStream() = default;
	virtual ~NullOutputStream() = default;

	virtual void write(uint8_t c) override;
	virtual void write(const uint8_t *src, size_t len) override;
	using OutputStream::write;

	size_t written() const;
	void reset();

	using OutputStream::flush;
	using OutputStream::close;
};


}


#endif
