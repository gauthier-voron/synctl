#include "synctl/io/OutputStream.hxx"

#include <cstdlib>
#include <string>


using std::string;
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

void OutputStream::writeStr(const string &str)
{
	write(str.c_str(), str.length() + 1);
}

void OutputStream::close()
{
}
