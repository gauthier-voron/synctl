#include "synctl/io/HashInputStream.hxx"

#include <openssl/sha.h>

#include <cstdint>
#include <cstdlib>

#include "synctl/io/IOException.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/tree/Reference.hxx"


using synctl::HashInputStream;
using synctl::IOException;
using synctl::InputStream;
using synctl::Reference;


HashInputStream::HashInputStream()
{
	if (SHA1_Init(&_ctx) == 0)
		throw IOException("SHA1_Init");
}

HashInputStream::HashInputStream(InputStream *inner, bool attached)
	: _inner(inner), _attached(attached)
{
	if (SHA1_Init(&_ctx) == 0)
		throw IOException("SHA1_Init");
}

HashInputStream::HashInputStream(HashInputStream &&other)
	: _inner(other._inner), _attached(other._attached)
{
	other._inner = nullptr;
	other._attached = false;

	if (SHA1_Init(&_ctx) == 0)
		throw IOException("SHA1_Init");
}

HashInputStream::~HashInputStream()
{
	unsigned char dump[SHA_DIGEST_LENGTH];

	SHA1_Final(dump, &_ctx);
	close();
}

HashInputStream &HashInputStream::operator=(HashInputStream &&other)
{
	_inner = other._inner;
	_attached = other._attached;

	other._inner = nullptr;
	other._attached = false;

	return *this;
}

void HashInputStream::assign(InputStream *inner)
{
	close();
	_inner = inner;
}

void HashInputStream::attach()
{
	if (_inner != nullptr)
		_attached = true;
}

void HashInputStream::attach(InputStream *inner)
{
	close();
	_inner = inner;
	_attached = (inner != nullptr);
}

void HashInputStream::detach()
{
	_attached = false;
}

void HashInputStream::digest(Reference *dest)
{
	if (SHA1_Final(dest->data(), &_ctx) == 0)
		throw IOException("SHA1_Final");
	if (SHA1_Init(&_ctx) == 0)
		throw IOException("SHA1_Init");
}

Reference HashInputStream::digest()
{
	Reference ret;

	digest(&ret);
	return ret;
}

int16_t HashInputStream::read()
{
	int16_t ret = _inner->read();
	uint8_t c;

	if (ret < 0)
		return ret;

	c = (uint8_t) ret;

	if (SHA1_Update(&_ctx, &c, 1) == 0)
		throw IOException("SHA1_Update");

	return ret;
}

size_t HashInputStream::read(uint8_t *dest, size_t len)
{
	size_t ret = _inner->read(dest, len);

	if (SHA1_Update(&_ctx, dest, ret) == 0)
		throw IOException("SHA1_Update");

	return ret;
}

void HashInputStream::close()
{
	if (_attached)
		_inner->close();
	_inner = nullptr;
	_attached = false;
}
