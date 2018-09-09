#ifndef _INCLUDE_SYNCTL_CLIENT_SENDER_HXX_
#define _INCLUDE_SYNCTL_CLIENT_SENDER_HXX_


#include <ostream>


namespace synctl {

class OutputStream;
class Reference;

}


namespace synctl::client {


class Sender
{
 public:
	virtual ~Sender() = default;

	virtual bool send(synctl::OutputStream *os, synctl::Reference *r) = 0;
};


}


#endif

