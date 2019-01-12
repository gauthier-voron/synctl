#ifndef _INCLUDE_SYNCTL_RECEIVER_HXX_
#define _INCLUDE_SYNCTL_RECEIVER_HXX_


#include <string>


namespace synctl {


class ReceiveContext;


class Receiver
{
 public:
	virtual ~Receiver();

	virtual void recv(const std::string &path, ReceiveContext *ctx) = 0;
};


}


#endif
