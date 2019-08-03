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


#ifndef _INCLUDE_SYNCTL_PROTOCOLVERSION_1_0_0_HXX_
#define _INCLUDE_SYNCTL_PROTOCOLVERSION_1_0_0_HXX_


#include <memory>

#include "synctl/plan/ProtocolVersion.hxx"


namespace synctl {


class Channel;
class Protocol;


class ProtocolVersion_1_0_0 : public ProtocolVersion
{
	ProtocolVersion_1_0_0();


 public:

	virtual std::unique_ptr<Protocol> getProtocol(Channel *chan) const;

	static const ProtocolVersion_1_0_0 *get();
};


}


#endif
