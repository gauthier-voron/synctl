#include "synctl/plan/ProtocolException.hxx"

#include <string>


using std::string;
using synctl::ProtocolException;


ProtocolException::ProtocolException()
{
}

ProtocolException::~ProtocolException()
{
}

ProtocolException::ProtocolException(const string &what)
	: _what(what)
{
}

string ProtocolException::what() const noexcept
{
	return _what;
}
