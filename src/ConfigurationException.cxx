#include "synctl/ConfigurationException.hxx"

#include <string>


using std::string;
using synctl::ConfigurationException;


ConfigurationException::ConfigurationException(const string &what)
	: _what(what)
{
}

const string &ConfigurationException::what() const noexcept
{
	return _what;
}
