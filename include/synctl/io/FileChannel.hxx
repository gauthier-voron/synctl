#ifndef _INCLUDE_SYNCTL_FILECHANNEL_HXX_
#define _INCLUDE_SYNCTL_FILECHANNEL_HXX_


#include <memory>


namespace synctl {


class Channel;


class FileChannel
{
 public:
	FileChannel() = delete;

	static std::unique_ptr<Channel> open(const std::string &path);
};


}


#endif
