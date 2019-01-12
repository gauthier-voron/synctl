#ifndef _INCLUDE_SYNCTL_OPERANDMISSINGEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_OPERANDMISSINGEXCEPTION_HXX_


#include <string>

#include "synctl/OperandException.hxx"


namespace synctl {


class OperandMissingException : public OperandException
{
 public:
	OperandMissingException() = default;
	OperandMissingException(const std::string &what);
};


}


#endif
