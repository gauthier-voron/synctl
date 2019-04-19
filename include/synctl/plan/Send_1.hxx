#ifndef _INCLUDE_SYNCTL_SEND_1_HXX_
#define _INCLUDE_SYNCTL_SEND_1_HXX_


#include <cstdint>
#include <string>

#include "synctl/plan/Opcode.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/repo/Snapshot.hxx"
#include "synctl/tree/Filter.hxx"
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
		opcode_t           opcode;
		Reference          reference;
		const Repository  *repository;
		Filter::Action     defact;
		OutputStream      *output;
	};


	Filter  *_filter = nullptr;


	Filter::Action _filterPath(const Context *context) const;

	void _sendObject(const Context *context);

	void _transfer(const Context *context, InputStream *input,
		       uint64_t size);

	void _sendDirectory(const Context *context, InputStream *input,
			    uint64_t size);

	void _sendLinktable(const Context *context);


 public:
	void setFilter(Filter *filter);

	void send(OutputStream *output, const Repository *repository,
		  const Snapshot::Content &content);
};


}


#endif
