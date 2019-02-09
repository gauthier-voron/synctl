#include "synctl/plan/GlobPattern.hxx"

#include <fnmatch.h>

#include <string>


using std::string;
using synctl::GlobPattern;


GlobPattern::GlobPattern()
{
}

GlobPattern::GlobPattern(const string &pattern)
	: _pattern(pattern)
{
}

bool GlobPattern::match(const string &path)
{
	return (fnmatch(_pattern.c_str(), path.c_str(), 0) == 0);
}
