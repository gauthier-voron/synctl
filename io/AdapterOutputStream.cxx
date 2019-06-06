#include "synctl/io/AdapterOutputStream.hxx"

#include <cstdint>
#include <cstdlib>
#include <ostream>

#include "synctl/io/EOFException.hxx"
#include "synctl/io/IOException.hxx"


using std::ostream;
using synctl::AdapterOutputStream;
using synctl::EOFException;
using synctl::IOException;


AdapterOutputStream::AdapterOutputStream()
{
}

AdapterOutputStream::AdapterOutputStream(ostream &inner, bool attached)
	: _inner(&inner), _attached(attached)
{
}

AdapterOutputStream::AdapterOutputStream(AdapterOutputStream &&other)
	: _inner(other._inner), _attached(other._attached)
{
	other._inner = nullptr;
	other._attached = false;
}

AdapterOutputStream &AdapterOutputStream::operator=(AdapterOutputStream &&o)
{
	_inner = o._inner;
	_attached = o._attached;

	o._inner = nullptr;
	o._attached = false;

	return *this;
}

void AdapterOutputStream::assign(ostream &inner)
{
	close();
	_inner = &inner;
}

void AdapterOutputStream::attach()
{
	if (_inner != nullptr)
		_attached = true;
}

void AdapterOutputStream::attach(std::ostream &inner)
{
	close();
	_inner = &inner;
	_attached = true;
}

void AdapterOutputStream::detach()
{
	_attached = false;
}

void AdapterOutputStream::write(uint8_t c)
{
	if (_inner->eof())
		throw EOFException();

	_inner->write(reinterpret_cast<char *> (&c), sizeof (c));

	if (_inner->bad())
		throw IOException();
}

void AdapterOutputStream::write(const uint8_t *src, size_t len)
{
	if (_inner->eof())
		throw EOFException();

	_inner->write(reinterpret_cast<const char *> (src), len);

	if (_inner->bad())
		throw IOException();
}

void AdapterOutputStream::flush()
{
	(*_inner) << std::flush;
}

void AdapterOutputStream::close()
{
	// if (_attached)
	// 	_inner->close();
	_inner = nullptr;
	_attached = false;
}
