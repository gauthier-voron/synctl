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
class Repository;


class Receive_1
{
	struct Baseref
	{
		Reference  reference;
		opcode_t   opcode;

		Baseref() = default;
		Baseref(const Reference &reference, opcode_t opcode);
		Baseref(const Baseref &other) = default;
		Baseref(Baseref &&other) = default;
	};

	struct Context
	{
		std::map<std::string, std::unique_ptr<Baseref>>    *baserefs;
		std::map<Reference, std::unique_ptr<Directory_1>>  *basedirs;
		std::map<Reference, std::unique_ptr<Directory_1>>  *recvdirs;
		InputStream                                        *input;
		Repository                                         *repository;
	};

	struct MergeContext
	{
		std::string     path;      // path of the merged directory
		Baseref        *baseref;   // ref/opcode in the base snapshot
		Directory_1    *basedir;   // directory in the base snapshot
		Directory_1    *recvdir;   // directory received from client
		const Context  *rcontext;
	};


	Snapshot::Content   _base;
	Filter             *_filter = nullptr;


	Filter::Action _filterPath(const std::string &path) const;

	Baseref *_loadBasedir(MergeContext *context, const std::string &name)
		const;

	void _findBasedir(MergeContext *context) const;

	void _loadRecvdir(MergeContext *context, const Reference &ref) const;

	void _findRecvdir(MergeContext *context, const Reference &ref) const;

	bool _loadBaseref(MergeContext *context) const;

	bool _findBaseref(MergeContext *context) const;


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
