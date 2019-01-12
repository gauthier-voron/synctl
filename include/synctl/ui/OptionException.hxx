#ifndef _INCLUDE_SYNCTL_OPTIONEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_OPTIONEXCEPTION_HXX_


#include <string>

#include "synctl/ui/ConfigurationException.hxx"


namespace synctl {


class OptionException : public ConfigurationException
{
 public:
	OptionException() = default;
	OptionException(const std::string &what);
};


}


#endif
