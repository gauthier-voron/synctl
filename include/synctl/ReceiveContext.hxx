#ifndef _INCLUDE_SYNCTL_RECEIVECONTEXT_HXX_
#define _INCLUDE_SYNCTL_RECEIVECONTEXT_HXX_


#include <string>


namespace synctl {


class InputStream;


class ReceiveContext
{
	std::string   _root;
	InputStream  *_stream;


 public:
	ReceiveContext();
	ReceiveContext(const ReceiveContext &other) = delete;
	ReceiveContext(const std::string &root, InputStream *stream);


	ReceiveContext &operator=(const ReceiveContext &other) = delete;


	InputStream *stream();

	const std::string &root() const;
};


}


#endif
