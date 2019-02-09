#ifndef _INCLUDE_SYNCTL_PUSH_1_HXX_
#define _INCLUDE_SYNCTL_PUSH_1_HXX_


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>
#include <string>

#include "synctl/plan/Filter.hxx"


namespace synctl {


class OutputStream;
class Reference;


class Push_1
{
	struct Context
	{
		std::string         apath;
		std::string         rpath;
		struct stat         stat;
		Filter::Action      defact;
		OutputStream       *output;
	};


	std::set<Reference>   _knownReferences;

	Filter               *_filter = nullptr;


	bool _isReferenceKnown(const Reference &reference) const;

	Filter::Action _filterPath(const Context *context) const;

	bool _pushEntry(const Context *context, Reference *reference);

	bool _pushDirectory(const Context *context, Reference *reference);

	bool _pushRegular(const Context *context, Reference *reference);

	bool _pushSymlink(const Context *context, Reference *reference);


 public:
	void addKnownReference(const Reference &reference);

	void setFilter(Filter *filter);

	void push(OutputStream *output, const std::string &root);
};


}


#endif
