#ifndef _INCLUDE_SYNCTL_REGULARV1_HXX_
#define _INCLUDE_SYNCTL_REGULARV1_HXX_


#include <string>

#include "synctl/Loader.hxx"
#include "synctl/Receiver.hxx"
#include "synctl/Sender.hxx"
#include "synctl/Storer.hxx"


namespace synctl {


class InputStream;
class LoadContext;
class OutputStream;
class ReceiveContext;
class Receiver;
class Reference;
class SendContext;
class StoreContext;


class RegularV1 : public Loader, Receiver, Sender, Storer
{
	void _transfer(OutputStream *out, InputStream *in, size_t len);


 public:
	virtual void recv(const std::string &path, ReceiveContext *ctx);

	virtual void send(Reference *dest, const std::string &path,
			  SendContext *ctx);

	virtual void load(InputStream *is, const std::string &path,
			  LoadContext *ctx);

	virtual void store(Reference *dest, StoreContext *ctx);
};


}


#endif
