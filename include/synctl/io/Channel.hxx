#ifndef _INCLUDE_SYNCTL_CHANNEL_HXX_
#define _INCLUDE_SYNCTL_CHANNEL_HXX_


#include <memory>


namespace synctl {


class InputStream;
class OutputStream;


/*
 * Two-side communication channel between a client and a server.
 * The open() static method parses the address and chooses what is the actual
 * implementation of the channel (file, ssh, https, ...).
 */
class Channel
{
 public:
	virtual InputStream *inputStream() = 0;
	virtual OutputStream *outputStream() = 0;

	virtual void close() = 0;

	static std::unique_ptr<Channel> open(const std::string &address);
};


}


#endif
