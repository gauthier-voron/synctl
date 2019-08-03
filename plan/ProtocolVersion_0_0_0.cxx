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


#include "synctl/plan/ProtocolVersion_0_0_0.hxx"

#include <memory>

#include "synctl/io/Channel.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/plan/ProtocolVersion.hxx"


using std::unique_ptr;
using synctl::Channel;
using synctl::Protocol;
using synctl::ProtocolVersion;
using synctl::ProtocolVersion_0_0_0;


ProtocolVersion_0_0_0::ProtocolVersion_0_0_0()
	: ProtocolVersion(0, 0, 0)
{
}

unique_ptr<Protocol> ProtocolVersion_0_0_0::getProtocol(Channel *chan [[gnu::unused]]) const
{
	return unique_ptr<Protocol>(new Protocol());
}

const ProtocolVersion_0_0_0 *ProtocolVersion_0_0_0::get()
{
	static ProtocolVersion_0_0_0 instance;
	return &instance;
}
