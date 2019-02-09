#ifndef _INCLUDE_SYNCTL_PROTOCOLEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_PROTOCOLEXCEPTION_HXX_


#include <string>


namespace synctl {


class ProtocolException
{
	std::string  _what;


 public:
	ProtocolException();
	ProtocolException(const std::string &what);
	ProtocolException(const ProtocolException &other) = default;
	ProtocolException(ProtocolException &&other) = default;
	virtual ~ProtocolException();

	ProtocolException &operator=(const ProtocolException &other) = default;
	ProtocolException &operator=(ProtocolException &&other) = default;

	std::string what() const noexcept;
};


}


#endif
