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
