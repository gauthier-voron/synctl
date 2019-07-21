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


#include "synctl/io/HashOutputStream.hxx"

#include <openssl/sha.h>

#include <cstdint>
#include <cstdlib>

#include "synctl/io/IOException.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Reference.hxx"


using synctl::HashOutputStream;
using synctl::IOException;
using synctl::OutputStream;
using synctl::Reference;


HashOutputStream::HashOutputStream()
{
	if (SHA1_Init(&_ctx) == 0)
		throw IOException("SHA1_Init");
}

HashOutputStream::HashOutputStream(OutputStream *inner, bool attached)
	: _inner(inner), _attached(attached)
{
	if (SHA1_Init(&_ctx) == 0)
		throw IOException("SHA1_Init");
}

HashOutputStream::HashOutputStream(HashOutputStream &&other)
	: _inner(other._inner), _attached(other._attached)
{
	other._inner = nullptr;
	other._attached = false;

	if (SHA1_Init(&_ctx) == 0)
		throw IOException("SHA1_Init");
}

HashOutputStream::~HashOutputStream()
{
	unsigned char dump[SHA_DIGEST_LENGTH];

	SHA1_Final(dump, &_ctx);
	close();
}

HashOutputStream &HashOutputStream::operator=(HashOutputStream &&other)
{
	_inner = other._inner;
	_attached = other._attached;

	other._inner = nullptr;
	other._attached = false;

	return *this;
}

void HashOutputStream::assign(OutputStream *inner)
{
	close();
	_inner = inner;
}

void HashOutputStream::attach()
{
	if (_inner != nullptr)
		_attached = true;
}

void HashOutputStream::attach(OutputStream *inner)
{
	close();
	_inner = inner;
	_attached = (inner != nullptr);
}

void HashOutputStream::detach()
{
	_attached = false;
}

void HashOutputStream::digest(Reference *dest)
{
	if (SHA1_Final(dest->data(), &_ctx) == 0)
		throw IOException("SHA1_Final");
	if (SHA1_Init(&_ctx) == 0)
		throw IOException("SHA1_Init");
}

Reference HashOutputStream::digest()
{
	Reference ret;

	digest(&ret);
	return ret;
}

void HashOutputStream::write(uint8_t c)
{
	if (SHA1_Update(&_ctx, &c, 1) == 0)
		throw IOException("SHA1_Update");
	if (_inner != nullptr)
		_inner->write(c);
}

void HashOutputStream::write(const uint8_t *src, size_t len)
{
	if (SHA1_Update(&_ctx, src, len) == 0)
		throw IOException("SHA1_Update");
	if (_inner != nullptr)
		_inner->write(src, len);
}

void HashOutputStream::flush()
{
	_inner->flush();
}

void HashOutputStream::close()
{
	if (_attached)
		_inner->close();
	_inner = nullptr;
	_attached = false;
}
