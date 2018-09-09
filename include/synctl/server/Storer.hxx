#ifndef _INCLUDE_SYNCTL_SERVER_STORER_HXX_
#define _INCLUDE_SYNCTL_SERVER_STORER_HXX_


#include <vector>

#include "synctl/Reference.hxx"


namespace synctl {

class InputStream;
class OutputStream;
class Reference;

}


namespace synctl::server {


class Repository;


class Storer
{
 public:
	virtual ~Storer() = default;

	virtual synctl::Reference
	store(synctl::InputStream *input, synctl::OutputStream *output,
	      std::vector<synctl::Reference> *children) = 0;
};


}


#endif
