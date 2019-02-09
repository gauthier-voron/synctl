#ifndef _INCLUDE_SYNCTL_GLOBPATTERN_HXX_
#define _INCLUDE_SYNCTL_GLOBPATTERN_HXX_


#include <string>

#include "synctl/plan/Pattern.hxx"


namespace synctl {


class GlobPattern : public Pattern
{
	std::string  _pattern;


 public:
	GlobPattern();
	GlobPattern(const std::string &pattern);
	GlobPattern(const GlobPattern &other) = default;
	virtual ~GlobPattern() = default;

	GlobPattern &operator=(const GlobPattern &other) = default;

	virtual bool match(const std::string &path);
};


}


#endif
