#include "synctl/io/IOException.hxx"

#include <string>


using std::string;
using synctl::IOException;


IOException::IOException()
{
}

IOException::~IOException()
{
}

IOException::IOException(const string &what)
	: _what(what)
{
}

string IOException::what() const noexcept
{
	return _what;
}
