#include "synctl/ui/OperandUnexpectedException.hxx"

#include <string>

#include "synctl/ui/OperandException.hxx"


using std::string;
using synctl::OperandException;
using synctl::OperandUnexpectedException;


OperandUnexpectedException::OperandUnexpectedException(const string &what)
	: OperandException(what)
{
}
