#include "synctl/io/Channel.hxx"

#include <memory>
#include <string>

#include "synctl/io/ChannelException.hxx"
#include "synctl/io/FileChannel.hxx"


using std::string;
using std::unique_ptr;
using synctl::Channel;
using synctl::ChannelException;
using synctl::FileChannel;


unique_ptr<Channel> Channel::open(const string &address)
{
	if (address.find("file://") == 0) {
		return FileChannel::open(address.substr(7));
	}

	throw ChannelException(address);
}
