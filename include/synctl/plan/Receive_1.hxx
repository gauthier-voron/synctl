#ifndef _INCLUDE_SYNCTL_RECEIVE_1_HXX_
#define _INCLUDE_SYNCTL_RECEIVE_1_HXX_


#include <map>
#include <memory>
#include <string>

#include "synctl/plan/Opcode.hxx"
#include "synctl/repo/Snapshot.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class Directory_1;
class InputStream;
class Link_1;
class Repository;
class SnapshotCombiner;


class Receive_1
{
	struct Context
	{
		SnapshotCombiner                                   *scombiner;
		InputStream                                        *input;
		Repository                                         *repository;
	};


	Snapshot::Content   _base;
	Filter             *_filter = nullptr;


	bool _receiveEntry(const Context *context);

	void _receiveDirectory(const Context *context);

	void _receiveRegular(const Context *context);

	void _receiveSymlink(const Context *context);

	bool _receiveLinks(const Context *context);

	void _receiveLink(const Context *context);

	void _receiveLinktable(const Context *context);


 public:
	void setBaseFilter(const Snapshot::Content &base, Filter *filter);

	void receive(InputStream *input, Repository *repository,
		     Snapshot::Content *content);
};


}


#endif
