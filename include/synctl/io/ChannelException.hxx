#ifndef _INCLUDE_SYNCTL_CHANNELEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_CHANNELEXCEPTION_HXX_


#include <string>


namespace synctl {


class ChannelException
{
	std::string  _what;


 public:
	ChannelException();
	ChannelException(const std::string &what);
	ChannelException(const ChannelException &other) = default;
	ChannelException(ChannelException &&other) = default;
	virtual ~ChannelException();

	ChannelException &operator=(const ChannelException &other) = default;
	ChannelException &operator=(ChannelException &&other) = default;

	std::string what() const noexcept;
};


}


#endif
