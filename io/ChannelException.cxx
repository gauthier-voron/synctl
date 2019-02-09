#include "synctl/io/ChannelException.hxx"

#include <string>


using std::string;
using synctl::ChannelException;


ChannelException::ChannelException()
{
}

ChannelException::~ChannelException()
{
}

ChannelException::ChannelException(const string &what)
	: _what(what)
{
}

string ChannelException::what() const noexcept
{
	return _what;
}
