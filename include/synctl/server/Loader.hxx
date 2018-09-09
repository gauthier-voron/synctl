#ifndef _INCLUDE_SYNCTL_SERVER_LOADER_HXX_
#define _INCLUDE_SYNCTL_SERVER_LOADER_HXX_


#include <vector>

#include "synctl/Reference.hxx"


namespace synctl {

class InputStream;
class OutputStream;

}


namespace synctl::server {


class Loader
{
 public:
	virtual void children(std::vector<synctl::Reference> *dest,
			      synctl::InputStream *input) = 0;
	std::vector<synctl::Reference> children(synctl::InputStream *input);
		
	virtual void load(synctl::InputStream *input,
			  synctl::OutputStream *output) = 0;
};


}


#endif
