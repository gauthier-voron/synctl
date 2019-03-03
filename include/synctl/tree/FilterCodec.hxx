#ifndef _INCLUDE_SYNCTL_FILTERCODEC_HXX_
#define _INCLUDE_SYNCTL_FILTERCODEC_HXX_


#include <memory>


namespace synctl {


class Filter;
class InputStream;
class OutputStream;


class FilterCodec
{
 public:
	FilterCodec() = default;


	void encode(const Filter *filer, OutputStream *output) const;

	std::unique_ptr<Filter> decode(InputStream *input) const;
};


}


#endif
