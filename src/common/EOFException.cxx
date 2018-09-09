#include "synctl/EOFException.hxx"

#include <string>


using std::string;
using synctl::EOFException;
using synctl::IOException;


EOFException::EOFException()
{
}

EOFException::EOFException(const string &what)
	: IOException(what)
{
}
