#ifndef _INCLUDE_SYNCTL_OPERANDINVALIDEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_OPERANDINVALIDEXCEPTION_HXX_


#include <string>

#include "synctl/OperandException.hxx"


namespace synctl {


class OperandInvalidException : public OperandException
{
	std::string  _opname;


 public:
	OperandInvalidException() = default;
	OperandInvalidException(const std::string &opname,
				const std::string &what);


	const std::string &opname() const noexcept;
};


}


#endif
