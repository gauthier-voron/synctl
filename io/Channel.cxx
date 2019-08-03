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
