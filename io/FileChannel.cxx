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
