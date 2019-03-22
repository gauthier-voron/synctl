#include "synctl/io/InputStream.hxx"

#include <cstdint>
#include <cstdlib>

#include "synctl/io/EOFException.hxx"
#include "synctl/io/IOException.hxx"


using std::string;
using synctl::EOFException;
using synctl::InputStream;
using synctl::IOException;


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
			throw IOException();
		dest[i] = static_cast<uint8_t> (buf);
	}

	return len;
}

void InputStream::readall(uint8_t *dest, size_t len)
{
	size_t s, done = 0;

	while (done < len) {
		s = read(dest + done, len - done);
		if (s == 0)
			throw EOFException();
		done += s;
	}
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
