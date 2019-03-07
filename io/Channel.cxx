#include "synctl/io/Channel.hxx"

#include <memory>
#include <string>

#include "synctl/io/ChannelException.hxx"
#include "synctl/io/FileChannel.hxx"
#include "synctl/io/SshChannel.hxx"


using std::string;
using std::unique_ptr;
using synctl::Channel;
using synctl::ChannelException;
using synctl::FileChannel;
using synctl::SshChannel;


unique_ptr<Channel> Channel::open(const string &address)
{
	SshChannel::OpenSettings sshSettings;

	if (address.find("file://") == 0) {
		return FileChannel::open(address.substr(7));
	} else if (address.find("ssh://") == 0) {
		sshSettings.location = address.substr(6);
		return SshChannel::open(sshSettings);
	}

	throw ChannelException(address);
}
