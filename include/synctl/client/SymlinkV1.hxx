#ifndef _INCLUDE_SYNCTL_CLIENT_SYMLINKV1_HXX_
#define _INCLUDE_SYNCTL_CLIENT_SYMLINKV1_HXX_


#include <istream>

#include "synctl/client/Receiver.hxx"
#include "synctl/client/Sender.hxx"


namespace synctl {

class OutputStream;
class Reference;

}


namespace synctl::client {


class SendContext;


class SymlinkV1 : public Receiver, public Sender
{
 public:
	virtual bool send(synctl::OutputStream *out, synctl::Reference *dest,
			  SendContext *context);

	virtual void recv(std::istream &is);
};


}


#endif
