#include "synctl/repo/RepositoryException.hxx"

#include <string>


using std::string;
using synctl::RepositoryException;


RepositoryException::RepositoryException()
{
}

RepositoryException::~RepositoryException()
{
}

RepositoryException::RepositoryException(const string &what)
	: _what(what)
{
}

string RepositoryException::what() const noexcept
{
	return _what;
}
