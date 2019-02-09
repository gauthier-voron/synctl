#ifndef _INCLUDE_SYNCTL_REPOSITORYEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_REPOSITORYEXCEPTION_HXX_


#include <string>


namespace synctl {


class RepositoryException
{
	std::string  _what;


 public:
	RepositoryException();
	RepositoryException(const std::string &what);
	RepositoryException(const RepositoryException &other) = default;
	RepositoryException(RepositoryException &&other) = default;
	virtual ~RepositoryException();

	RepositoryException &operator=(const RepositoryException &o) = default;
	RepositoryException &operator=(RepositoryException &&other) = default;

	std::string what() const noexcept;
};


}


#endif
