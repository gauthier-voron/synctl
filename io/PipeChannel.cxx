#include "synctl/io/PipeChannel.hxx"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

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
using std::vector;
using synctl::Channel;
using synctl::IOException;
using synctl::InputStream;
using synctl::OutputStream;
using synctl::PipeChannel;
using synctl::Protocol;


PipeChannel::PipeChannel(FdInputStream &&in, FdOutputStream &&out, pid_t pid)
	: _in(move(in)), _out(move(out)), _pid(pid)
{
}

InputStream *PipeChannel::inputStream()
{
	return &_in;
}

OutputStream *PipeChannel::outputStream()
{
	return &_out;
}

void PipeChannel::close()
{
	int status;

	_in.close();
	_out.close();

	if (_pid != 0)
		::waitpid(_pid, &status, 0);
}

[[noreturn]] static void __execCommand(const vector<string> &cmd,
				       int stdin, int stdout)
{
	const char **argv = new const char *[cmd.size() + 1];
	size_t i;

	for (i = 0; i < cmd.size(); i++)
		argv[i] = cmd[i].c_str();
	argv[cmd.size()] = nullptr;

	::dup2(stdin, STDIN_FILENO);
	::dup2(stdout, STDOUT_FILENO);
	::execvp(cmd[0].c_str(), (char *const *) argv);
	::exit(1);
}

unique_ptr<Channel> PipeChannel::open(const vector<string> &cmd)
{
	unique_ptr<PipeChannel> chan;
	FdInputStream fromServer;
	FdOutputStream toServer;
	int fromServerPipe[2];
	int toServerPipe[2];
	pid_t commandPid;
	int ret;

	ret = pipe(fromServerPipe);
	if (ret != 0)
		throw IOException();

	ret = pipe(toServerPipe);
	if (ret != 0)
		throw IOException();

	commandPid = ::fork();
	if (commandPid == 0) {
		::close(toServerPipe[1]);
		::close(fromServerPipe[0]);
		__execCommand(cmd, toServerPipe[0], fromServerPipe[1]);
	} else if (commandPid < 0) {
		throw IOException();
	} else {
		::close(toServerPipe[0]);
		::close(fromServerPipe[1]);
	}

	fromServer = FdInputStream(fromServerPipe[0]);
	toServer = FdOutputStream(toServerPipe[1]);

	chan = unique_ptr<PipeChannel>(new PipeChannel(move(fromServer),
						       move(toServer),
						       commandPid));

	return chan;
}
