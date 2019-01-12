#ifndef _INCLUDE_SYNCTL_OPTIONWITHOUTARGUMENTEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_OPTIONWITHOUTARGUMENTEXCEPTION_HXX_


#include <string>

#include "synctl/OptionException.hxx"


namespace synctl {


class OptionWithoutArgumentException : public OptionException
{
 public:
	OptionWithoutArgumentException() = default;
	OptionWithoutArgumentException(const std::string &what);
};


}


#endif
