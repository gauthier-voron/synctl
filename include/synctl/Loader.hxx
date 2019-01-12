#ifndef _INCLUDE_SYNCTL_LOADER_HXX_
#define _INCLUDE_SYNCTL_LOADER_HXX_


#include <string>


namespace synctl {


class InputStream;
class LoadContext;


class Loader
{
 public:
	virtual ~Loader();

	virtual void load(InputStream *is, const std::string &path,
			  LoadContext *ctx) = 0;
};


}


#endif
