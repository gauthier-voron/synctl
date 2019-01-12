#include "synctl/OperandUnexpectedException.hxx"

#include <string>

#include "synctl/OperandException.hxx"


using std::string;
using synctl::OperandException;
using synctl::OperandUnexpectedException;


OperandUnexpectedException::OperandUnexpectedException(const string &what)
	: OperandException(what)
{
}
