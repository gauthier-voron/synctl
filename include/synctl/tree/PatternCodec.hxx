#ifndef _INCLUDE_SYNCTL_PATTERNCODEC_HXX_
#define _INCLUDE_SYNCTL_PATTERNCODEC_HXX_


#include <memory>


namespace synctl {


class Pattern;
class InputStream;
class OutputStream;


class PatternCodec
{
 public:
	PatternCodec() = default;


	void encode(const Pattern *filer, OutputStream *output) const;

	std::unique_ptr<Pattern> decode(InputStream *input) const;
};


}


#endif
