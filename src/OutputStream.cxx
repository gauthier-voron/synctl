#include "synctl/OutputStream.hxx"

#include <cstdlib>


using synctl::OutputStream;


OutputStream::~OutputStream()
{
	close();
}

void OutputStream::write(const uint8_t *src, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++)
		write(src[i]);
}

void OutputStream::close()
{
}
