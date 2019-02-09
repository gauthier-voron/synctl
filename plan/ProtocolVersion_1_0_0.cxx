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
