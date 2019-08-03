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


#ifndef _INCLUDE_SYNCTL_HASHOUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_HASHOUTPUTSTREAM_HXX_


#include <openssl/sha.h>

#include <cstdint>
#include <cstdlib>

#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class HashOutputStream : public OutputStream
{
	SHA_CTX        _ctx;
	OutputStream  *_inner = nullptr;
	bool           _attached = false;


 public:
	HashOutputStream();
	HashOutputStream(OutputStream *inner, bool attached = false);
	HashOutputStream(const HashOutputStream &other) = delete;
	HashOutputStream(HashOutputStream &&other);
	virtual ~HashOutputStream();

	HashOutputStream &operator=(const HashOutputStream &other) = delete;
	HashOutputStream &operator=(HashOutputStream &&other);

	void assign(OutputStream *inner);

	void attach();
	void attach(OutputStream *inner);

	void detach();

	void digest(Reference *dest);
	Reference digest();

	virtual void write(uint8_t c) override;
	virtual void write(const uint8_t *src, size_t len) override;
	using OutputStream::write;

	virtual void flush() override;

	virtual void close() override;
};


}


#endif
