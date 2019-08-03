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


#ifndef _INCLUDE_SYNCTL_LIMITEDINPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_LIMITEDINPUTSTREAM_HXX_


#include <cstdint>

#include <memory>

#include "synctl/io/InputStream.hxx"


namespace synctl {


class LimitedInputStream : public InputStream
{
	std::unique_ptr<InputStream>   _attached;
	InputStream                   *_inner = nullptr;
	size_t                         _limit = 0;


 public:
	LimitedInputStream() = default;
	LimitedInputStream(InputStream *inner, size_t limit);
	LimitedInputStream(std::unique_ptr<InputStream> &&inner, size_t limit);
	LimitedInputStream(LimitedInputStream &&other) = default;
	virtual ~LimitedInputStream() = default;

	LimitedInputStream &operator=(LimitedInputStream &&other) = default;


	void reset(size_t limit);
	size_t remains() const;

	void attach(InputStream *inner);
	void attach(std::unique_ptr<InputStream> &&inner);


	virtual int16_t read();
	virtual size_t read(uint8_t *dest, size_t len);
	using InputStream::readall;

	virtual void close();
};


}


#endif
