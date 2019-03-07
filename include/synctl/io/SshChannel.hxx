#ifndef _INCLUDE_SYNCTL_SSHCHANNEL_HXX_
#define _INCLUDE_SYNCTL_SSHCHANNEL_HXX_


#include <memory>
#include <vector>


namespace synctl {


class Channel;


class SshChannel
{
 public:
	struct OpenSettings
	{
		// Mandatory location where to open the ssh channel.
		// Specified in the following form:
		//
		//     [user@]address[:port]/[~[user]/]path
		//
		std::string               location;

		// Command executed on the remote side to launch the ssh
		// channel server.
		// If left empty, the default command is:
		//
		//     { "synctl" , "server" }
		//
		std::vector<std::string>  remoteCommand;
	};

	SshChannel() = delete;

	static std::unique_ptr<Channel> open(const OpenSettings &settings);
};


}


#endif
