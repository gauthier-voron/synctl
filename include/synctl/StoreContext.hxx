#ifndef _INCLUDE_SYNCTL_STORECONTEXT_HXX_
#define _INCLUDE_SYNCTL_STORECONTEXT_HXX_


namespace synctl {


class InputStream;
class Repository;


class StoreContext
{
	Repository   *_repository;
	InputStream  *_stream;


 public:
	StoreContext();
	StoreContext(const StoreContext &other) = delete;
	StoreContext(Repository *repository, InputStream *input);


	StoreContext &operator=(const StoreContext &other) = delete;


	InputStream *stream();

	Repository *repository();
};


}


#endif
