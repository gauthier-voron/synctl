#ifndef _INCLUDE_SYNCTL_PROTOCOL_1_0_0_HXX_
#define _INCLUDE_SYNCTL_PROTOCOL_1_0_0_HXX_


#include <vector>

#include "synctl/plan/Protocol.hxx"


namespace synctl {


class Channel;
class ProtocolVersion;
class ProtocolVersion_1_0_0;
class Reference;
class Repository;


class Protocol_1_0_0 : public Protocol
{
	friend class ProtocolVersion_1_0_0;


	Channel *_channel = nullptr;


	Protocol_1_0_0(Channel *channel);

	void _serveList(Repository *repository) const;

	void _servePush(Repository *repository) const;

	void _servePull(Repository *repository) const;


 public:
	virtual ~Protocol_1_0_0() = default;


	Protocol_1_0_0 &operator=(const Protocol_1_0_0 &other) = default;
	Protocol_1_0_0 &operator=(Protocol_1_0_0 &&other) = default;


	virtual const ProtocolVersion *version() const noexcept;


	virtual void exit() const override;


	virtual void list(const ListSettings &settings) const override;

	virtual void push(const PushSettings &settings) const override;

	virtual void pull(const PullSettings &settings) const override;


	virtual void serve(Repository *repository) const override;
};


}


#endif
