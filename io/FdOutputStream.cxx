#include "synctl/io/FdOutputStream.hxx"

#include <unistd.h>

#include <cstdint>
#include <cstdlib>

#include "synctl/io/IOException.hxx"


using std::string;
using synctl::FdOutputStream;
using synctl::IOException;


FdOutputStream::FdOutputStream()
{
}

FdOutputStream::FdOutputStream(int fd)
	: _fd(fd)
{
}

FdOutputStream::FdOutputStream(FdOutputStream &&other)
	: _fd(other._fd)
{
	other._fd = -1;
}

FdOutputStream::~FdOutputStream()
{
	close();
}

FdOutputStream &FdOutputStream::operator=(FdOutputStream &&other)
{
	close();
	_fd = other._fd;
	other._fd = -1;
	return *this;
}

void FdOutputStream::write(uint8_t c)
{
	write(&c, sizeof (c));
}

void FdOutputStream::write(const uint8_t *src, size_t len)
{
	ssize_t ssize;

	ssize = ::write(_fd, src, len);
	if ((ssize < 0) || (((size_t) ssize) != len))
		throw IOException("cannot write properly");
}

void FdOutputStream::close()
{
	if (_fd >= 0)
		::close(_fd);
	_fd = -1;
}
