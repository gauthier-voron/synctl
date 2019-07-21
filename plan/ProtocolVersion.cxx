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


#include "synctl/plan/ProtocolVersion.hxx"

#include <memory>

#include "synctl/io/Channel.hxx"
#include "synctl/plan/Protocol.hxx"


using std::unique_ptr;
using synctl::Channel;
using synctl::Protocol;
using synctl::ProtocolVersion;


ProtocolVersion::ProtocolVersion(uint16_t major, uint16_t minor,
				 uint16_t variant)
	: _major(major), _minor(minor), _variant(variant)
{
}

uint16_t ProtocolVersion::major() const noexcept
{
	return _major;
}

uint16_t ProtocolVersion::minor() const noexcept
{
	return _minor;
}

uint16_t ProtocolVersion::variant() const noexcept
{
	return _variant;
}

bool ProtocolVersion::isCompatible(const ProtocolVersion &other) const noexcept
{
	return isCompatible(other._major, other._minor, other._variant);
}

bool ProtocolVersion::isCompatible(uint16_t major, uint16_t minor,
				   uint16_t variant [[gnu::unused]])
	const noexcept
{
	if (major != _major)
		return false;
	if (minor > _minor)
		return false;
	return true;
}
