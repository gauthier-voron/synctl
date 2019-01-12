#include "synctl/OptionException.hxx"

#include <string>

#include "synctl/ConfigurationException.hxx"


using std::string;
using synctl::ConfigurationException;
using synctl::OptionException;


OptionException::OptionException(const string &what)
	: ConfigurationException(what)
{
}
