#include "synctl/FileOutputStream.hxx"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <string>

#include "synctl/IOException.hxx"


using std::string;
using synctl::FileOutputStream;
using synctl::IOException;


FileOutputStream::FileOutputStream()
{
}

FileOutputStream::FileOutputStream(const string path)
{
	_fd = open(path.c_str(), O_WRONLY | O_CREAT, 0600);
	if (_fd == -1)
		throw IOException("cannot open file");
}

FileOutputStream::FileOutputStream(FileOutputStream &&other)
	: _fd(other._fd)
{
	other._fd = -1;
}

FileOutputStream::~FileOutputStream()
{
	close();
}

FileOutputStream &FileOutputStream::operator=(FileOutputStream &&other)
{
	_fd = other._fd;
	other._fd = -1;
	return *this;
}

void FileOutputStream::write(uint8_t c)
{
	write(&c, sizeof (c));
}

void FileOutputStream::write(const uint8_t *src, size_t len)
{
	ssize_t ssize;

	ssize = ::write(_fd, src, len);
	if ((ssize < 0) || (((size_t) ssize) != len))
		throw IOException("cannot write properly");
}

void FileOutputStream::close()
{
	if (_fd >= 0)
		::close(_fd);
}
