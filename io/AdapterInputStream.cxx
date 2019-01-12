#include "synctl/io/AdapterInputStream.hxx"

#include <cstdint>
#include <cstdlib>
#include <istream>

#include "synctl/io/EOFException.hxx"
#include "synctl/io/IOException.hxx"


using std::istream;
using synctl::AdapterInputStream;
using synctl::EOFException;
using synctl::IOException;


AdapterInputStream::AdapterInputStream()
{
}

AdapterInputStream::AdapterInputStream(istream &inner, bool attached)
	: _inner(&inner), _attached(attached)
{
}

AdapterInputStream::AdapterInputStream(AdapterInputStream &&other)
	: _inner(other._inner), _attached(other._attached)
{
	other._inner = nullptr;
	other._attached = false;
}

AdapterInputStream &AdapterInputStream::operator=(AdapterInputStream &&o)
{
	_inner = o._inner;
	_attached = o._attached;

	o._inner = nullptr;
	o._attached = false;

	return *this;
}

void AdapterInputStream::assign(istream &inner)
{
	close();
	_inner = &inner;
}

void AdapterInputStream::attach()
{
	if (_inner != nullptr)
		_attached = true;
}

void AdapterInputStream::attach(istream &inner)
{
	close();
	_inner = &inner;
	_attached = true;
}

void AdapterInputStream::detach()
{
	_attached = false;
}

int16_t AdapterInputStream::read()
{
	uint8_t c;

	if (_inner->eof())
		throw EOFException();

	_inner->read(reinterpret_cast<char *> (&c), sizeof (c));
	if (_inner->gcount() == 0)
		return -1;

	if (_inner->bad())
		throw IOException();

	return c;
}

size_t AdapterInputStream::read(uint8_t *dest, size_t len)
{
	if (_inner->eof())
		throw EOFException();

	_inner->read(reinterpret_cast<char *> (dest), len);
	return _inner->gcount();

	if (_inner->bad())
		throw IOException();
}

void AdapterInputStream::close()
{
	// if (_attached)
	// 	_inner->close();
	_inner = nullptr;
	_attached = false;
}
