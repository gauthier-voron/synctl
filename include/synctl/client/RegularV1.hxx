#ifndef _INCLUDE_SYNCTL_CLIENT_REGULARV1_HXX_
#define _INCLUDE_SYNCTL_CLIENT_REGULARV1_HXX_


#include <istream>

#include "synctl/client/Receiver.hxx"
#include "synctl/client/Sender.hxx"


namespace synctl {

class OutputStream;
class Reference;

}


namespace synctl::client {


class SendContext;


class RegularV1 : public Receiver, public Sender
{
	size_t _transfer(synctl::OutputStream *to, std::istream &sr, size_t n);


 public:
	virtual bool send(OutputStream *out, synctl::Reference *dest,
			  SendContext *context);

	using Sender::traverse;


	virtual void recv(std::istream &is);
};


}


#endif
