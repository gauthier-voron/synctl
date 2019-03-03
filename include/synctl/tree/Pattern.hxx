#ifndef _INCLUDE_SYNCTL_PATTERN_HXX_
#define _INCLUDE_SYNCTL_PATTERN_HXX_


#include <string>


namespace synctl {


class InputStream;
class OutputStream;


class Pattern
{
 public:
	virtual ~Pattern() = default;

	virtual bool match(const std::string &path) = 0;

	virtual void write(OutputStream *output) const = 0;

	virtual void read(InputStream *input) = 0;
};


}


#endif
