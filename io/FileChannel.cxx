#include "synctl/io/FileChannel.hxx"

#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <memory>
#include <string>

#include "synctl/io/Channel.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/io/FdInputStream.hxx"
#include "synctl/io/FdOutputStream.hxx"
#include "synctl/io/IOException.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"


using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using synctl::Channel;
using synctl::FileChannel;
using synctl::IOException;
using synctl::InputStream;
using synctl::OutputStream;
using synctl::Protocol;


FileChannel::FileChannel(FdInputStream &&in, FdOutputStream &&out)
	: _in(move(in)), _out(move(out))
{
}

InputStream *FileChannel::inputStream()
{
	return &_in;
}

OutputStream *FileChannel::outputStream()
{
	return &_out;
}

void FileChannel::close()
{
	_in.close();
	_out.close();
}

static string __readlink(const string &path)
{
	ssize_t s, l = 64;
	string buffer;

	buffer.resize(l);

	while ((s = ::readlink(path.c_str(), buffer.data(), l)) == l) {
		l = l << 1;
		buffer.resize(l);
	}

	if (s < 0)
		throw IOException(path);

	buffer.resize(s);
	return buffer;
}

[[noreturn]] static void __execServer(int stdin, int stdout,
				      const string &path)
{
	string execPath = __readlink("/proc/self/exe");
	const char *argv[4];

	argv[0] = execPath.c_str();
	argv[1] = "server";
	argv[2] = path.c_str();
	argv[3] = nullptr;
 
	::dup2(stdin, STDIN_FILENO);
	::dup2(stdout, STDOUT_FILENO);
	::execv(execPath.c_str(), (char *const *) argv);
	::exit(1);
}

unique_ptr<Channel> FileChannel::open(const string &path)
{
	unique_ptr<FileChannel> chan;
	FdInputStream fromServer;
	FdOutputStream toServer;
	pid_t serverPid;
	int fromServerPipe[2];
	int toServerPipe[2];
	int ret;

	ret = pipe(fromServerPipe);
	if (ret != 0)
		throw IOException();

	ret = pipe(toServerPipe);
	if (ret != 0)
		throw IOException();

	serverPid = ::fork();
	if (serverPid == 0) {
		::close(toServerPipe[1]);
		::close(fromServerPipe[0]);
		__execServer(toServerPipe[0], fromServerPipe[1], path);
	} else if (serverPid < 0) {
		throw IOException();
	} else {
		::close(toServerPipe[0]);
		::close(fromServerPipe[1]);
	}

	fromServer = FdInputStream(fromServerPipe[0]);
	toServer = FdOutputStream(toServerPipe[1]);

	chan = unique_ptr<FileChannel>
		(new FileChannel(move(fromServer), move(toServer)));

	return chan;
}
