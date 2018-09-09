#ifndef _INCLUDE_SYNCTL_CLIENT_DIRECTORYV1_HXX_
#define _INCLUDE_SYNCTL_CLIENT_DIRECTORYV1_HXX_


#include <sys/stat.h>
#include <sys/types.h>

#include <istream>
#include <ostream>
#include <string>

#include "synctl/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/client/Receiver.hxx"
#include "synctl/client/Sender.hxx"


namespace synctl::client {


class EntryFactory;


class DirectoryV1 : public Receiver, public Sender
{
	const EntryFactory  *_factory;
	std::string          _path;


	bool _send_child(synctl::OutputStream *os, const std::string &path,
			 synctl::Reference *ref) const;

	void _send_child_meta(synctl::OutputStream *os,
			      const std::string &name, 
			      const synctl::Reference &ref,
			      const struct stat &st) const;


 public:
	DirectoryV1(const std::string &path, const EntryFactory *factory);

	virtual bool send(synctl::OutputStream *os, synctl::Reference *dest);
	virtual void recv(std::istream &is);
};


}


#endif
