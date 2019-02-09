#ifndef _INCLUDE_SYNCTL_FSEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_FSEXCEPTION_HXX_


#include <string>

#include "synctl/io/IOException.hxx"


namespace synctl {


class FSException : public IOException
{
 public:
	FSException();
	FSException(const std::string &what);
	FSException(const FSException &other) = default;
	FSException(FSException &&other) = default;
	virtual ~FSException() = default;

	FSException &operator=(const FSException &other) = default;
	FSException &operator=(FSException &&other) = default;

	using IOException::what;
};


}


#endif
