#ifndef _INCLUDE_SYCNTL_SERVER_DIRECTORYV1_HXX_
#define _INCLUDE_SYCNTL_SERVER_DIRECTORYV1_HXX_


#include <vector>

#include "synctl/Reference.hxx"
#include "synctl/server/Loader.hxx"
#include "synctl/server/Storer.hxx"


namespace synctl {

class InputStream;
class OutputStream;

}


namespace synctl::server {


class DirectoryV1 : public Loader, public Storer
{
	void _transfer(synctl::InputStream *in, synctl::OutputStream *out,
		       synctl::Reference *ref);


 public:
	DirectoryV1();


	virtual void children(std::vector<synctl::Reference> *dest,
			      synctl::InputStream *input);
	using Loader::children;
		
	virtual void load(synctl::InputStream *input,
			  synctl::OutputStream *output);

	virtual synctl::Reference
	store(synctl::InputStream *input, synctl::OutputStream *output,
	      std::vector<synctl::Reference> *children);
};


}


#endif
