#ifndef _INCLUDE_SYNCTL_CONFIGURATIONEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATIONEXCEPTION_HXX_


#include <string>


namespace synctl {


class ConfigurationException
{
	std::string  _what;

 public:
	ConfigurationException() = default;
	ConfigurationException(const std::string &what);

	const std::string &what() const noexcept;
};


}


#endif
