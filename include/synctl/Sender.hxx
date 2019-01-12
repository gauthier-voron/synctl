#ifndef _INCLUDE_SYNCTL_SENDER_HXX_
#define _INCLUDE_SYNCTL_SENDER_HXX_


#include <string>


namespace synctl {


class Reference;
class SendContext;


class Sender
{
 public:
	virtual ~Sender();

	virtual void send(Reference *dest, const std::string &path,
			  SendContext *ctx) = 0;
};


}


#endif
