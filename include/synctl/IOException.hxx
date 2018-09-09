#ifndef _INCLUDE_SYNCTL_IOEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_IOEXCEPTION_HXX_


#include <string>


namespace synctl {


class IOException
{
	std::string  _what;


 public:
	IOException();
	IOException(const std::string &what);
	IOException(const IOException &other) = default;
	IOException(IOException &&other) = default;
	virtual ~IOException();

	IOException &operator=(const IOException &other) = default;
	IOException &operator=(IOException &&other) = default;

	std::string what() const noexcept;
};


}


#endif
