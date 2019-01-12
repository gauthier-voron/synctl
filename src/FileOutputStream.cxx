#include "synctl/FileOutputStream.hxx"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <string>

#include "synctl/FdOutputStream.hxx"
#include "synctl/IOException.hxx"


using std::move;
using std::string;
using synctl::FdOutputStream;
using synctl::FileOutputStream;
using synctl::IOException;


FileOutputStream::FileOutputStream()
	: FdOutputStream()
{
}

FileOutputStream::FileOutputStream(const string path)
{
	int fd = open(path.c_str(), O_WRONLY | O_CREAT, 0600);

	if (fd == -1)
		throw IOException("cannot open file");

	FdOutputStream::operator=(FdOutputStream(fd));
}

FileOutputStream::FileOutputStream(FileOutputStream &&other)
	: FdOutputStream(move(other))
{
}

FileOutputStream &FileOutputStream::operator=(FileOutputStream &&other)
{
	FdOutputStream::operator=(move(other));
	return *this;
}
