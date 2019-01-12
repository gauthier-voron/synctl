#ifndef _INCLUDE_SYNCTL_OPERANDUNEXPECTEDEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_OPERANDUNEXPECTEDEXCEPTION_HXX_


#include <string>

#include "synctl/ui/OperandException.hxx"


namespace synctl {


class OperandUnexpectedException : public OperandException
{
 public:
	OperandUnexpectedException() = default;
	OperandUnexpectedException(const std::string &what);
};


}


#endif
