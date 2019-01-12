#include "synctl/ui/OptionUnknownException.hxx"

#include <string>

#include "synctl/ui/OptionUnknownException.hxx"


using std::string;
using synctl::OptionException;
using synctl::OptionUnknownException;


OptionUnknownException::OptionUnknownException(const string &what)
	: OptionException(what)
{
}
