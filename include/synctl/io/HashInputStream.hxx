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


#ifndef _INCLUDE_SYNCTL_HASHINPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_HASHINPUTSTREAM_HXX_


#include <openssl/sha.h>

#include <cstdint>
#include <cstdlib>

#include "synctl/io/InputStream.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class HashInputStream : public InputStream
{
	SHA_CTX       _ctx;
	InputStream  *_inner = nullptr;
	bool          _attached = false;


 public:
	HashInputStream();
	HashInputStream(InputStream *inner, bool attached = false);
	HashInputStream(const HashInputStream &other) = delete;
	HashInputStream(HashInputStream &&other);
	virtual ~HashInputStream();

	HashInputStream &operator=(const HashInputStream &other) = delete;
	HashInputStream &operator=(HashInputStream &&other);

	void assign(InputStream *inner);

	void attach();
	void attach(InputStream *inner);

	void detach();

	void digest(Reference *dest);
	Reference digest();

	virtual int16_t read();
	virtual size_t read(uint8_t *dest, size_t len);
	using InputStream::readall;

	virtual void close();
};


}


#endif
