#ifndef _INCLUDE_SYNCTL_CLIENT_REGULARV1_HXX_
#define _INCLUDE_SYNCTL_CLIENT_REGULARV1_HXX_


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


class EntryFactory;


class RegularV1 : public Receiver, public Sender
{
	const EntryFactory  *_factory;
	std::string          _path;


	size_t _transfer(synctl::OutputStream *to, std::istream &sr, size_t n);


 public:
	RegularV1(const std::string &path, const EntryFactory *factory);

	virtual bool send(OutputStream *os, synctl::Reference *dest);
	virtual void recv(std::istream &is);
};


}


#endif
