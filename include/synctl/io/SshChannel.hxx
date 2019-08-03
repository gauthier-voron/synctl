// Copyright 2019 Gauthier Voron
//
// This file is part of Synctl
//
// Synctl is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Fundation, either version 3 of the License, or (at your option) any later
// version.
//
// Synctl is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANDABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// Synctl. If not, see <https://www.gnu.org/licenses/>.
//


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
