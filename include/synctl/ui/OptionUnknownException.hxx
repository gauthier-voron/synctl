#ifndef _INCLUDE_SYNCTL_OPTIONUNKNOWNEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_OPTIONUNKNOWNEXCEPTION_HXX_


#include <string>

#include "synctl/ui/OptionException.hxx"


namespace synctl {


class OptionUnknownException : public OptionException
{
 public:
	OptionUnknownException() = default;
	OptionUnknownException(const std::string &what);
};


}


#endif
