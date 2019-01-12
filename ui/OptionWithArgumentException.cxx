#include "synctl/ui/OptionWithArgumentException.hxx"

#include <string>

#include "synctl/ui/OptionWithArgumentException.hxx"


using std::string;
using synctl::OptionException;
using synctl::OptionWithArgumentException;


OptionWithArgumentException::OptionWithArgumentException(const string &what)
	: OptionException(what)
{
}
