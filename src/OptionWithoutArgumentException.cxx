#include "synctl/OptionWithoutArgumentException.hxx"

#include <string>

#include "synctl/OptionWithoutArgumentException.hxx"


using std::string;
using synctl::OptionException;
using synctl::OptionWithoutArgumentException;


OptionWithoutArgumentException::OptionWithoutArgumentException
(const string &what)
	: OptionException(what)
{
}
