#ifndef _INCLUDE_SYNCTL_PUSH_1_HXX_
#define _INCLUDE_SYNCTL_PUSH_1_HXX_


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>
#include <string>

#include "synctl/io/LinkTracker.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Filter.hxx"


namespace synctl {


class OutputStream;
class Reference;


class Push_1
{
	struct EntryId
	{
		dev_t  dev;
		ino_t  ino;

		EntryId(const struct stat &stat);
		EntryId(EntryId &&other);

		bool operator<(const EntryId &other) const;
	};

	struct Context
	{
		std::string      apath;
		std::string      rpath;
		struct stat      stat;
		Filter::Action   defact;
		LinkTracker     *links;
		OutputStream    *output;
	};


	std::set<Reference>   _knownReferences;

	Filter               *_filter = nullptr;


	bool _isReferenceKnown(const Reference &reference) const;

	Filter::Action _filterPath(const Context *context) const;

	bool _pushEntry(const Context *context, Reference *reference,
			opcode_t *opcode);

	bool _pushDirectory(const Context *context, Reference *reference,
			    opcode_t *opcode);

	bool _pushRegular(const Context *context, Reference *reference,
			  opcode_t *opcode);

	bool _pushSymlink(const Context *context, Reference *reference,
			  opcode_t *opcode);

	void _pushLinks(const Context *context, Reference *reference);


 public:
	void addKnownReference(const Reference &reference);

	void setFilter(Filter *filter);

	void push(OutputStream *output, const std::string &root);
};


}


#endif
