#include "synctl/ui/OperandMissingException.hxx"

#include <string>

#include "synctl/ui/OperandException.hxx"


using std::string;
using synctl::OperandException;
using synctl::OperandMissingException;


OperandMissingException::OperandMissingException(const string &what)
	: OperandException(what)
{
}
