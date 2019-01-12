#include "synctl/OptionWithArgumentException.hxx"

#include <string>

#include "synctl/OptionWithArgumentException.hxx"


using std::string;
using synctl::OptionException;
using synctl::OptionWithArgumentException;


OptionWithArgumentException::OptionWithArgumentException(const string &what)
	: OptionException(what)
{
}
