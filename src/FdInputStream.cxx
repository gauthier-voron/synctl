#include "synctl/FdInputStream.hxx"

#include <unistd.h>

#include "synctl/EOFException.hxx"
#include "synctl/IOException.hxx"


using synctl::EOFException;
using synctl::FdInputStream;
using synctl::IOException;


FdInputStream::FdInputStream()
{
}

FdInputStream::FdInputStream(int fd)
	: _fd(fd)
{
}

FdInputStream::FdInputStream(FdInputStream &&other)
	: _fd(other._fd)
{
	other._fd = -1;
}

FdInputStream::~FdInputStream()
{
	close();
}

FdInputStream &FdInputStream::operator=(FdInputStream &&other)
{
	close();
	_fd = other._fd;
	other._fd = -1;
	return *this;
}

int16_t FdInputStream::read()
{
	uint8_t val;
	size_t done = read(&val, sizeof (val));

	if (done == 0)
		return -1;
	return val;
}

size_t FdInputStream::read(uint8_t *dest, size_t len)
{
	ssize_t ssize = ::read(_fd, dest, len);

	if (ssize < 0)
		throw IOException("cannot read properly");

	return ((size_t) ssize);
}

void FdInputStream::readall(uint8_t *dest, size_t len)
{
	size_t r, done = 0;

	while (done < len) {
		r = read(dest + done, len - done);
		if (r == 0)
			throw EOFException("cannot read properly");
		done += r;
	}
}

void FdInputStream::close()
{
	if (_fd >= 0)
		::close(_fd);
	_fd = -1;
}
