#include "synctl/OptionUnknownException.hxx"

#include <string>

#include "synctl/OptionUnknownException.hxx"


using std::string;
using synctl::OptionException;
using synctl::OptionUnknownException;


OptionUnknownException::OptionUnknownException(const string &what)
	: OptionException(what)
{
}
