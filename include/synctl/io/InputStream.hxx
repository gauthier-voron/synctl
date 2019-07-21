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


#ifndef _INCLUDE_SYNCTL_INPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_INPUTSTREAM_HXX_


#include <endian.h>

#include <cstdint>
#include <cstdlib>
#include <string>


namespace synctl {


class InputStream
{
 public:
	virtual ~InputStream();

	virtual int16_t read() = 0;
	virtual size_t read(uint8_t *dest, size_t len);
	virtual void readall(uint8_t *dest, size_t len);

	template<typename T> size_t read(T *dest, size_t len);
	template<typename T> void readall(T *dest, size_t len);

	virtual std::string readStr();
	virtual void readStr(std::string *dest);

	template<typename T> T readInt();

	virtual void close();
};


}


template<typename T>
size_t synctl::InputStream::read(T *dest, size_t len)
{
	return read(reinterpret_cast<uint8_t *> (dest), len);
}

template<typename T>
void synctl::InputStream::readall(T *dest, size_t len)
{
	readall(reinterpret_cast<uint8_t *> (dest), len);
}

template<>
inline uint8_t synctl::InputStream::readInt<uint8_t>()
{
	uint8_t ret;
	readall(&ret, sizeof (ret));
	return ret;
}

template<>
inline uint16_t synctl::InputStream::readInt<uint16_t>()
{
	uint16_t ret;
	readall(reinterpret_cast<uint8_t *> (&ret), sizeof (ret));
	return le16toh(ret);
}

template<>
inline uint32_t synctl::InputStream::readInt<uint32_t>()
{
	uint32_t ret;
	readall(reinterpret_cast<uint8_t *> (&ret), sizeof (ret));
	return le32toh(ret);
}

template<>
inline uint64_t synctl::InputStream::readInt<uint64_t>()
{
	uint64_t ret;
	readall(reinterpret_cast<uint8_t *> (&ret), sizeof (ret));
	return le64toh(ret);
}


#endif
