#ifndef _INCLUDE_SYNCTL_PATTERN_HXX_
#define _INCLUDE_SYNCTL_PATTERN_HXX_


#include <string>


namespace synctl {


class Pattern
{
 public:
	virtual ~Pattern() = default;

	virtual bool match(const std::string &path) = 0;
};


}


#endif
