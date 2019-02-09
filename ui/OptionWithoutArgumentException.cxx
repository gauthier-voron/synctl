#include "synctl/ui/OptionWithoutArgumentException.hxx"

#include <string>

#include "synctl/ui/OptionWithoutArgumentException.hxx"


using std::string;
using synctl::OptionException;
using synctl::OptionWithoutArgumentException;


OptionWithoutArgumentException::OptionWithoutArgumentException
(const string &what)
	: OptionException(what)
{
}
