#ifndef _INCLUDE_SYNCTL_PROTOCOLVERSION_HXX_
#define _INCLUDE_SYNCTL_PROTOCOLVERSION_HXX_


#include <memory>


namespace synctl {


class Channel;
class Protocol;


class ProtocolVersion
{
	const uint16_t  _major;
	const uint16_t  _minor;
	const uint16_t  _variant;


 protected:
	ProtocolVersion(uint16_t major, uint16_t minor, uint16_t variant);


 public:
	// Return the major version of this protocol.
	// Two protocols with different major versions are never compatible.
	uint16_t major() const noexcept;

	// Return the minor version of this protocol.
	// Given two protocols with the same major version, the protocol with
	// minor version A is compatible with protocol with minor version B if
	// B <= A.
	// That is, a client protocol always has a minor version lesser or
	// equals to the server minor version.
	uint16_t minor() const noexcept;

	// Return the variant version of this protocol.
	// Two protocols with compatible major and minor versions are always
	// compatible, whatever their variant versions.
	// Two protocols with different variants may however show different
	// performace.
	uint16_t variant() const noexcept;

	// Indicate if all the operations of the protocol of this version are
	// supported by the protocol of the other version.
	// This operation is not always symetric.
	bool isCompatible(const ProtocolVersion &other) const noexcept;
	bool isCompatible(uint16_t major, uint16_t minor, uint16_t variant)
		const noexcept;

	// Create a new instance of the associated protocol for the given
	// channel.
	virtual std::unique_ptr<Protocol> getProtocol(Channel *chan) const = 0;
};


}


#endif
