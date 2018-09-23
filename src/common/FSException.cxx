#include "synctl/FSException.hxx"

#include <string>


using std::string;
using synctl::FSException;
using synctl::IOException;


FSException::FSException()
{
}

FSException::FSException(const string &what)
	: IOException(what)
{
}
