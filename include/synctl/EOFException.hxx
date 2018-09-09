#ifndef _INCLUDE_SYNCTL_EOFEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_EOFEXCEPTION_HXX_


#include <string>

#include "synctl/IOException.hxx"


namespace synctl {


class EOFException : public IOException
{
 public:
	EOFException();
	EOFException(const std::string &what);
	EOFException(const EOFException &other) = default;
	EOFException(EOFException &&other) = default;
	virtual ~EOFException() = default;

	EOFException &operator=(const EOFException &other) = default;
	EOFException &operator=(EOFException &&other) = default;

	using IOException::what;
};


}


#endif
