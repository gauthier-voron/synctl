#ifndef _INCLUDE_SYNCTL_SERVER_SYMLINKV1_HXX_
#define _INCLUDE_SYNCTL_SERVER_SYMLINKV1_HXX_


#include <vector>

#include "synctl/Reference.hxx"
#include "synctl/server/Loader.hxx"
#include "synctl/server/Storer.hxx"


namespace synctl {

class InputStream;
class OutputStream;
class Reference;

}


namespace synctl::server {


class SymlinkV1 : public Storer, public Loader
{
 public:
	SymlinkV1();


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
