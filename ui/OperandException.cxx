#include "synctl/ui/OperandException.hxx"

#include <string>

#include "synctl/ui/ConfigurationException.hxx"


using std::string;
using synctl::ConfigurationException;
using synctl::OperandException;


OperandException::OperandException(const string &what)
	: ConfigurationException(what)
{
}
