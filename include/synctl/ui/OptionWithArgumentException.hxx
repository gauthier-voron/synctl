#ifndef _INCLUDE_SYNCTL_OPTIONWITHARGUMENTEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_OPTIONWITHARGUMENTEXCEPTION_HXX_


#include <string>

#include "synctl/ui/OptionException.hxx"


namespace synctl {


class OptionWithArgumentException : public OptionException
{
 public:
	OptionWithArgumentException() = default;
	OptionWithArgumentException(const std::string &what);
};


}


#endif
