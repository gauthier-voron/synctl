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
class SendContext;


class DirectoryV1 : public Receiver, public Sender
{
	bool _send_child(synctl::OutputStream *out,
			 const std::string &name,
			 synctl::Reference *dest,
			 SendContext *context) const;

	void _send_child_meta(synctl::OutputStream *bout,
			      const std::string &name, 
			      const synctl::Reference &ref,
			      const struct stat &st) const;

	size_t _send_children(synctl::OutputStream *out,
			      synctl::OutputStream *bout,
			      SendContext *context) const;

	bool _send(synctl::OutputStream *out, synctl::Reference *dest,
		   SendContext *context, bool traverse) const;


 public:
	virtual bool send(synctl::OutputStream *out, synctl::Reference *dest,
			  SendContext *context);

	virtual bool traverse(synctl::OutputStream *out, synctl::Reference *ds,
			      SendContext *context);

	virtual void recv(std::istream &is);
};


}


#endif
