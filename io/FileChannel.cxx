#include "synctl/io/FileChannel.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/Channel.hxx"
#include "synctl/io/PipeChannel.hxx"
#include "synctl/io/Symlink.hxx"


using std::string;
using std::unique_ptr;
using std::vector;
using synctl::Channel;
using synctl::FileChannel;
using synctl::PipeChannel;
using synctl::Symlink;


unique_ptr<Channel> FileChannel::open(const string &path)
{
	Symlink link = Symlink("/proc/self/exe");
	vector<string> command;

	command.push_back(link.get());
	command.push_back("server");
	command.push_back(path);

	return PipeChannel::open(command);
}
