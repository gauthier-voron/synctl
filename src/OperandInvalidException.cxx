#include "synctl/OperandInvalidException.hxx"

#include <string>

#include "synctl/OperandException.hxx"


using std::string;
using synctl::OperandException;
using synctl::OperandInvalidException;


OperandInvalidException::OperandInvalidException(const string &opname,
						 const string &what)
	: OperandException(what), _opname(opname)
{
}

const string &OperandInvalidException::opname() const noexcept
{
	return _opname;
}
