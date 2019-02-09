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
