#ifndef _INCLUDE_SYNCTL_OPERANDEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_OPERANDEXCEPTION_HXX_


#include <string>

#include "synctl/ConfigurationException.hxx"


namespace synctl {


class OperandException : public ConfigurationException
{
 public:
	OperandException() = default;
	OperandException(const std::string &what);
};


}


#endif
