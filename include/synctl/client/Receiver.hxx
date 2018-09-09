#ifndef _INCLUDE_SYNCTL_CLIENT_RECEIVER_HXX_
#define _INCLUDE_SYNCTL_CLIENT_RECEIVER_HXX_


#include <istream>


namespace synctl::client {


class Receiver
{
 public:
	virtual void recv(std::istream &is) = 0;
};


}


#endif
