#include "synctl/OperandMissingException.hxx"

#include <string>

#include "synctl/OperandException.hxx"


using std::string;
using synctl::OperandException;
using synctl::OperandMissingException;


OperandMissingException::OperandMissingException(const string &what)
	: OperandException(what)
{
}
