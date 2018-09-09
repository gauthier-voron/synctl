#include "synctl/FileInputStream.hxx"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <string>

#include "synctl/EOFException.hxx"
#include "synctl/IOException.hxx"


using std::string;
using synctl::FileInputStream;
using synctl::EOFException;
using synctl::IOException;


FileInputStream::FileInputStream()
{
}

FileInputStream::FileInputStream(const string path)
{
	_fd = open(path.c_str(), O_RDONLY);
	if (_fd == -1)
		throw IOException("cannot open file");
}

FileInputStream::FileInputStream(FileInputStream &&other)
	: _fd(other._fd)
{
	other._fd = -1;
}

FileInputStream::~FileInputStream()
{
	close();
}

FileInputStream &FileInputStream::operator=(FileInputStream &&other)
{
	_fd = other._fd;
	other._fd = -1;
	return *this;
}

int16_t FileInputStream::read()
{
	uint8_t val;
	size_t done = read(&val, sizeof (val));

	if (done == 0)
		return -1;
	return val;
}

size_t FileInputStream::read(uint8_t *dest, size_t len)
{
	ssize_t ssize = ::read(_fd, dest, len);

	if (ssize < 0)
		throw IOException("cannot read properly");

	return ((size_t) ssize);
}

void FileInputStream::readall(uint8_t *dest, size_t len)
{
	size_t r, done = 0;

	while (done < len) {
		r = read(dest + done, len - done);
		if (r == 0)
			throw EOFException("cannot read properly");
		done += r;
	}
}

void FileInputStream::close()
{
	if (_fd >= 0)
		::close(_fd);
	_fd = -1;
}
