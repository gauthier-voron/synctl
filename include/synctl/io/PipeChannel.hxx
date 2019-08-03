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


#ifndef _INCLUDE_SYNCTL_PIPECHANNEL_HXX_
#define _INCLUDE_SYNCTL_PIPECHANNEL_HXX_


#include <sys/types.h>
#include <unistd.h>

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/Channel.hxx"
#include "synctl/io/FdInputStream.hxx"
#include "synctl/io/FdOutputStream.hxx"


namespace synctl {


class PipeChannel : public Channel
{
	FdInputStream   _in;
	FdOutputStream  _out;
	pid_t           _pid;


 public:
	PipeChannel(FdInputStream &&in, FdOutputStream &&out, pid_t pid = 0);
	virtual ~PipeChannel() = default;

	virtual InputStream *inputStream();
	virtual OutputStream *outputStream();

	virtual void close();

	static std::unique_ptr<Channel>
	open(const std::vector<std::string> &command);
};


}


#endif

