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


#include "synctl/plan/ProtocolVersion_1_0_0.hxx"

#include <memory>

#include "synctl/io/Channel.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/plan/Protocol_1_0_0.hxx"
#include "synctl/plan/ProtocolVersion.hxx"


using std::unique_ptr;
using synctl::Channel;
using synctl::Protocol;
using synctl::Protocol_1_0_0;
using synctl::ProtocolVersion_1_0_0;


ProtocolVersion_1_0_0::ProtocolVersion_1_0_0()
	: ProtocolVersion(1, 0, 0)
{
}

unique_ptr<Protocol> ProtocolVersion_1_0_0::getProtocol(Channel *chan) const
{
	return unique_ptr<Protocol_1_0_0>(new Protocol_1_0_0(chan));
}

const ProtocolVersion_1_0_0 *ProtocolVersion_1_0_0::get()
{
	static ProtocolVersion_1_0_0 instance;
	return &instance;
}
