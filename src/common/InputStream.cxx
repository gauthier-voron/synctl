#include "synctl/InputStream.hxx"

#include <cstdint>
#include <cstdlib>

#include "synctl/EOFException.hxx"


using synctl::EOFException;
using synctl::InputStream;


InputStream::~InputStream()
{
	close();
}

size_t InputStream::read(uint8_t *dest, size_t len)
{
	size_t i;
	int16_t buf;

	for (i = 0; i < len; i++) {
		buf = read();
		if (buf == -1)
			return i;
		dest[i] = static_cast<uint8_t> (buf);
	}

	return len;
}

void InputStream::readall(uint8_t *dest, size_t len)
{
	if (read(dest, len) != len)
		throw EOFException();
}

void InputStream::close()
{
}
