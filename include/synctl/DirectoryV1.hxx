#ifndef _INCLUDE_SYNCTL_DIRECTORYV1_HXX
#define _INCLUDE_SYNCTL_DIRECTORYV1_HXX


#include <sys/stat.h>
#include <sys/types.h>

#include <string>
#include <vector>

#include "synctl/Loader.hxx"
#include "synctl/Receiver.hxx"
#include "synctl/Sender.hxx"
#include "synctl/Storer.hxx"


namespace synctl {


class InputStream;
class OutputStream;
class Receiver;
class Reference;
class SendContext;
class StoreContext;


class DirectoryV1 : public Loader, Receiver, Sender, Storer
{
	size_t _recvChildMeta(struct stat *st, Reference *ref,
			      std::string *name, ReceiveContext *ctx);

	void _recvAddChild(const std::string &path, const struct stat &st,
			   const Reference &ref, ReceiveContext *ctx);

	void _recvDelChild(const std::string &path, ReceiveContext *ctx);
	
	void _recvUpdateChild(const std::string &path, const struct stat &st,
			      const Reference &ref, ReceiveContext *ctx);

	void _recvUpdateMeta(const std::string &path, const struct stat &st,
			     ReceiveContext *ctx);

	bool _sendChild(Reference *dest, const std::string &path,
			SendContext *ctx, const struct stat &st);

	size_t _sendChildMeta(OutputStream *out, const std::string &name,
			      const Reference &ref, const struct stat &st);

	size_t _sendChildren(const std::string &path, SendContext *ctx,
			     OutputStream *meta);


	size_t _loadChildMeta(std::vector<Reference> *refs,
			      std::vector<std::string> *paths, InputStream *is,
			      const std::string &path, LoadContext *ctx);

	void _loadChild(const Reference &ref, const std::string &path,
			LoadContext *ctx);


	bool _storeChild(StoreContext *ctx);

	void _storeChildrenMeta(Reference *dest, StoreContext *ctx);


 public:
	virtual void recv(const std::string &path, ReceiveContext *ctx);

	virtual void load(InputStream *is, const std::string &path,
			  LoadContext *ctx);

	virtual void send(Reference *dest, const std::string &path,
			  SendContext *ctx);

	virtual void store(Reference *dest, StoreContext *ctx);
};


}


#endif
