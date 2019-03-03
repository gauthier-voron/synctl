#ifndef _INCLUDE_SYNCTL_GLOBPATTERN_HXX_
#define _INCLUDE_SYNCTL_GLOBPATTERN_HXX_


#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>

#include <string>

#include "synctl/tree/Pattern.hxx"


namespace synctl {


class GlobPattern : public Pattern
{
	::pcre2_code  *_pcreCode = nullptr;
	std::string    _perlRegex;


 public:
	GlobPattern();
	GlobPattern(const std::string &pattern);
	GlobPattern(const GlobPattern &other) = delete;
	GlobPattern(GlobPattern &&other);
	virtual ~GlobPattern();

	GlobPattern &operator=(const GlobPattern &other) = default;
	GlobPattern &operator=(GlobPattern &&other);

	virtual bool match(const std::string &path) override;

	virtual void write(OutputStream *output) const override;

	virtual void read(InputStream *input) override;
};


}


#endif
