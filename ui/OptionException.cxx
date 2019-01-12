#include "synctl/ui/OptionException.hxx"

#include <string>

#include "synctl/ui/ConfigurationException.hxx"


using std::string;
using synctl::ConfigurationException;
using synctl::OptionException;


OptionException::OptionException(const string &what)
	: ConfigurationException(what)
{
}
