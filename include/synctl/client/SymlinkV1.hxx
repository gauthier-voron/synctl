#ifndef _INCLUDE_SYNCTL_CLIENT_SYMLINKV1_HXX_
#define _INCLUDE_SYNCTL_CLIENT_SYMLINKV1_HXX_


#include <istream>
#include <ostream>
#include <string>

#include "synctl/client/Receiver.hxx"
#include "synctl/client/Sender.hxx"


namespace synctl {

class OutputStream;
class Reference;

}


namespace synctl::client {


class SymlinkV1 : public Receiver, public Sender
{
	std::string          _path;


 public:
	SymlinkV1(const std::string &path);

	virtual bool send(synctl::OutputStream *os, synctl::Reference *dest);
	virtual void recv(std::istream &is);
};


}


#endif
