#ifndef _INCLUDE_SYNCTL_FILECHANNEL_HXX_
#define _INCLUDE_SYNCTL_FILECHANNEL_HXX_


#include <memory>

#include "synctl/io/Channel.hxx"
#include "synctl/io/FdInputStream.hxx"
#include "synctl/io/FdOutputStream.hxx"


namespace synctl {


class FileChannel : public Channel
{
	FdInputStream   _in;
	FdOutputStream  _out;


 public:
	FileChannel(FdInputStream &&in, FdOutputStream &&out);
	virtual ~FileChannel() = default;

	virtual InputStream *inputStream();
	virtual OutputStream *outputStream();

	virtual void close();

	static std::unique_ptr<Channel> open(const std::string &path);
};


}


#endif
