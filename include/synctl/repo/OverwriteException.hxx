#ifndef _INCLUDE_SYNCTL_OVERWRITEEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_OVERWRITEEXCEPTION_HXX_


#include <string>

#include "synctl/repo/RepositoryException.hxx"


namespace synctl {


class OverwriteException : public RepositoryException
{
 public:
	OverwriteException() = default;
	OverwriteException(const std::string &what);
	OverwriteException(const OverwriteException &other) = default;
	OverwriteException(OverwriteException &&other) = default;
	virtual ~OverwriteException() = default;

	OverwriteException &operator=(const OverwriteException &o) = default;
	OverwriteException &operator=(OverwriteException &&other) = default;

	using RepositoryException::what;
};


}


#endif
