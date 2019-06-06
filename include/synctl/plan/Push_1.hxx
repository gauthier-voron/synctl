#ifndef _INCLUDE_SYNCTL_PUSH_1_HXX_
#define _INCLUDE_SYNCTL_PUSH_1_HXX_


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>
#include <string>

#include "synctl/io/LinkTracker.hxx"
#include "synctl/plan/LoggerPush0_1.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Filter.hxx"


namespace synctl {


class LoggerPush_1;
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

	LoggerPush0_1         _deflogger;
	LoggerPush_1         *_logger = &_deflogger;


	bool _isReferenceKnown(const Reference &reference) const;

	Filter::Action _filterPath(const Context *context) const;

	void _fixAccessTime(const Context *context);

	bool _pushEntry(const Context *context, Reference *reference,
			opcode_t *opcode);

	bool _pushDirectory(const Context *context, Reference *reference,
			    opcode_t *opcode);

	bool _pushRegular(const Context *context, Reference *reference,
			  opcode_t *opcode);

	bool _pushSymlink(const Context *context, Reference *reference,
			  opcode_t *opcode);


 public:
	void addKnownReference(const Reference &reference);

	void setFilter(Filter *filter);

	void setLogger(LoggerPush_1 *logger);

	void push(OutputStream *output, const std::string &root);
};


}


#endif
