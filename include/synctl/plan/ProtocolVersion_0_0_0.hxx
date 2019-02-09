#ifndef _INCLUDE_SYNCTL_PROTOCOLVERSION_0_0_0_HXX_
#define _INCLUDE_SYNCTL_PROTOCOLVERSION_0_0_0_HXX_


#include <memory>

#include "synctl/plan/ProtocolVersion.hxx"


namespace synctl {


class Channel;
class Protocol;


class ProtocolVersion_0_0_0 : public ProtocolVersion
{
	ProtocolVersion_0_0_0();


 public:
	virtual std::unique_ptr<Protocol> getProtocol(Channel *chan) const;

	static const ProtocolVersion_0_0_0 *get();
};


}


#endif
