#ifndef _INCLUDE_SYNCTL_PUSH_1_HXX_
#define _INCLUDE_SYNCTL_PUSH_1_HXX_


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>
#include <string>


namespace synctl {


class Filter;
class OutputStream;
class Reference;


class Push_1
{
	struct Context
	{
		std::string         path;
		struct stat         stat;
		const std::string  *root;
		OutputStream       *output;
	};


	std::set<Reference>   _knownReferences;


	bool _isReferenceKnown(const Reference &reference) const;

	void _pushEntry(const Context *context, Reference *reference);

	void _pushDirectory(const Context *context, Reference *reference);

	void _pushRegular(const Context *context, Reference *reference);

	void _pushSymlink(const Context *context, Reference *reference);


 public:
	void addKnownReference(const Reference &reference);

	void addFilter(Filter *filter);

	void push(OutputStream *output, const std::string &root);
};


}


#endif
