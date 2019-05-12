#ifndef _INCLUDE_SYNCTL_LINKMERGER_HXX_
#define _INCLUDE_SYNCTL_LINKMERGER_HXX_


#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "synctl/plan/Explorer.hxx"
#include "synctl/repo/Snapshot.hxx"


namespace synctl {


class Link_1;
class Linktable_1;
class Reference;
class Repository;


class LinkBuilder
{
	struct Link : public std::set<std::string>
	{
		Explorer::Entry  entry;
	};

	using LinkPtr = std::unique_ptr<Link>;

	using LinkSet = std::set<Link *>;

	struct MergeLeaves
	{
		std::set<std::string>    merged;
		std::set<std::string>    received;
		const Explorer::Entry   *recventry;

		void clear();
	};


	Repository                      *_repository;
	Explorer                        *_explorer;
	std::unique_ptr<Explorer>        _exprivate;

	std::vector<LinkPtr>             _rstorage;
	std::map<std::string, Link *>    _received;
	std::set<std::string>            _merged;

	std::vector<LinkPtr>             _bstorage;
	std::map<std::string, LinkSet>   _baseprefix;


	void _loadBaseLink(const Snapshot::Content &base,
			   const Link_1 &link);

	void _loadBaseTable(const Snapshot::Content &base,
			    const Linktable_1 &table);

	void _mergeLeaves(const MergeLeaves &leaves,
			  const Snapshot::Content &snapshot);

	void _mergeNode(const std::string &path,
			const Snapshot::Content &snapshot);

	void _mergeNodes(const Snapshot::Content &snapshot);

	void _buildLink(Link *link, Reference *dest);


 public:
	LinkBuilder() = default;
	LinkBuilder(Repository *repository, Explorer *explorer = nullptr);


	void bindReceived(const std::string &path0, const std::string &path1);

	void mergePath(const std::string &path);

	void loadBase(const Snapshot::Content &base);

	void buildLinktable(Snapshot::Content *dest);
};


}


#endif