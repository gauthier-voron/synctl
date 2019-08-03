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


#ifndef _INCLUDE_SYNCTL_OUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_OUTPUTSTREAM_HXX_


#include <endian.h>

#include <cstdint>
#include <cstdlib>
#include <string>


namespace synctl {


class OutputStream
{
 public:
	virtual ~OutputStream();

	virtual void write(uint8_t c) = 0;
	virtual void write(const uint8_t *src, size_t len);
	virtual void write(const std::string &str);

	template<typename T> void write(const T *src, size_t len);

	virtual void writeStr(const std::string &str);

	template<typename T> void writeInt(T num);

	virtual void flush();

	virtual void close();
};


}


template<typename T>
void synctl::OutputStream::write(const T *src, size_t len)
{
	write(reinterpret_cast<const uint8_t *> (src), len);
}

template<>
inline void synctl::OutputStream::writeInt<uint8_t>(uint8_t num)
{
	write(&num, sizeof (num));
}

template<>
inline void synctl::OutputStream::writeInt<uint16_t>(uint16_t num)
{
	num = htole16(num);
	write(&num, sizeof (num));
}

template<>
inline void synctl::OutputStream::writeInt<uint32_t>(uint32_t num)
{
	num = htole32(num);
	write(&num, sizeof (num));
}

template<>
inline void synctl::OutputStream::writeInt<uint64_t>(uint64_t num)
{
	num = htole64(num);
	write(&num, sizeof (num));
}


#endif
