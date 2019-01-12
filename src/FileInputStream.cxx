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


using std::move;
using std::string;
using synctl::FileInputStream;
using synctl::EOFException;
using synctl::IOException;


FileInputStream::FileInputStream()
	: FdInputStream()
{
}

FileInputStream::FileInputStream(const string path)
{
	int fd = open(path.c_str(), O_RDONLY);

	if (fd == -1)
		throw IOException("cannot open file");

	FdInputStream::operator=(FdInputStream(fd));
}

FileInputStream::FileInputStream(FileInputStream &&other)
	: FdInputStream(move(other))
{
}

FileInputStream &FileInputStream::operator=(FileInputStream &&other)
{
	FdInputStream::operator=(move(other));
	return *this;
}
