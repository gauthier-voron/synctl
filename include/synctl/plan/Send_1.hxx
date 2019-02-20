#ifndef _INCLUDE_SYNCTL_SEND_1_HXX_
#define _INCLUDE_SYNCTL_SEND_1_HXX_


#include <string>

#include "synctl/plan/Filter.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class InputStream;
class OutputStream;
class Repository;


class Send_1
{
	struct Context
	{
		std::string        rpath;
		Reference          reference;
		const Repository  *repository;
		Filter::Action     defact;
		OutputStream      *output;
	};


	Filter  *_filter = nullptr;


	Filter::Action _filterPath(const Context *context) const;

	void _sendObject(const Context *context);

	void _transfer(const Context *context, InputStream *input);

	void _sendDirectory(const Context *context, InputStream *input);


 public:
	void setFilter(Filter *filter);

	void send(OutputStream *output, const Repository *repository,
		  const Reference &root);
};


}


#endif