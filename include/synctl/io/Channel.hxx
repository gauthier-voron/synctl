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


#ifndef _INCLUDE_SYNCTL_CHANNEL_HXX_
#define _INCLUDE_SYNCTL_CHANNEL_HXX_


#include <memory>


namespace synctl {


class InputStream;
class OutputStream;


/*
 * Two-side communication channel between a client and a server.
 * The open() static method parses the address and chooses what is the actual
 * implementation of the channel (file, ssh, https, ...).
 */
class Channel
{
 public:
	virtual InputStream *inputStream() = 0;
	virtual OutputStream *outputStream() = 0;

	virtual void close() = 0;

	static std::unique_ptr<Channel> open(const std::string &address);
};


}


#endif
