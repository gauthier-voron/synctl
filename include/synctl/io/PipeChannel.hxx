#ifndef _INCLUDE_SYNCTL_PIPECHANNEL_HXX_
#define _INCLUDE_SYNCTL_PIPECHANNEL_HXX_


#include <sys/types.h>
#include <unistd.h>

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/Channel.hxx"
#include "synctl/io/FdInputStream.hxx"
#include "synctl/io/FdOutputStream.hxx"


namespace synctl {


class PipeChannel : public Channel
{
	FdInputStream   _in;
	FdOutputStream  _out;
	pid_t           _pid;


 public:
	PipeChannel(FdInputStream &&in, FdOutputStream &&out, pid_t pid = 0);
	virtual ~PipeChannel() = default;

	virtual InputStream *inputStream();
	virtual OutputStream *outputStream();

	virtual void close();

	static std::unique_ptr<Channel>
	open(const std::vector<std::string> &command);
};


}


#endif

