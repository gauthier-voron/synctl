#include "synctl/io/NullOutputStream.hxx"

#include <cstdint>


using synctl::NullOutputStream;


void NullOutputStream::write(uint8_t c [[gnu::unused]])
{
	_size += 1;
}

void NullOutputStream::write(const uint8_t *src [[gnu::unused]], size_t len)
{
	_size += len;
}

size_t NullOutputStream::written() const
{
	return _size;
}

void NullOutputStream::reset()
{
	_size = 0;
}

void NullOutputStream::close()
{
}
