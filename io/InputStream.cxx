#include "synctl/io/InputStream.hxx"

#include <cstdint>
#include <cstdlib>

#include "synctl/io/EOFException.hxx"


using std::string;
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

string InputStream::readStr()
{
	string ret;
	readStr(&ret);
	return ret;
}

void InputStream::readStr(string *dest)
{
	int16_t buf;

	dest->clear();

	while ((buf = read()) > 0)
		dest->push_back(static_cast<char> (buf));

	if (buf != 0)
		throw EOFException();
}

void InputStream::close()
{
}
