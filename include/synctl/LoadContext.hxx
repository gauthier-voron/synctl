#ifndef _INCLUDE_SYNCTL_LOADCONTEXT_HXX_
#define _INCLUDE_SYNCTL_LOADCONTEXT_HXX_


#include <string>


namespace synctl {


class Filter;
class OutputStream;
class Repository;


class LoadContext
{
	Repository    *_repository;
	OutputStream  *_stream;
	Filter        *_filter;


 public:
	LoadContext();
	LoadContext(const LoadContext &other) = delete;
	LoadContext(Repository *repository, OutputStream *stream,
		    Filter *filter);


	LoadContext &operator=(const LoadContext &other) = delete;


	OutputStream *stream();

	bool contains(const std::string &path);

	Repository *repository();
};


}


#endif
