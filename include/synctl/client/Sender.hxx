#ifndef _INCLUDE_SYNCTL_CLIENT_SENDER_HXX_
#define _INCLUDE_SYNCTL_CLIENT_SENDER_HXX_


#include <ostream>


namespace synctl {

class OutputStream;
class Reference;

}


namespace synctl::client {


class SendContext;


class Sender
{
 public:
	virtual ~Sender() = default;

	virtual bool send(synctl::OutputStream *out, synctl::Reference *dest,
			  SendContext *context) = 0;

	virtual bool traverse(synctl::OutputStream *out, synctl::Reference *ds,
			      SendContext *context);
};


}


#endif

