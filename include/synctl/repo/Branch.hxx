#ifndef _INCLUDE_SYNCTL_BRANCH_HXX_
#define _INCLUDE_SYNCTL_BRANCH_HXX_


#include <memory>
#include <string>
#include <vector>

#include "synctl/io/Directory.hxx"
#include "synctl/repo/Snapshot.hxx"


namespace synctl {


class Reference;


class Branch
{
	Directory                                       _dir;
	mutable std::vector<std::unique_ptr<Snapshot>>  _snapshots;
	mutable std::vector<Snapshot *>                 _rwptrs;
	mutable std::vector<const Snapshot *>           _roptrs;
	mutable bool                                    _loaded = false;


	void _ensureLoaded() const;

	std::string _newSnapshotPath() const;
	Snapshot *_newSnapshot(const std::string &path) const;
	Snapshot *_newSnapshot() const;


 public:
	Branch(const std::string &path);


	void initialize() const;
	void load() const;


	Snapshot *newSnapshot(const Reference &ref);
	Snapshot *newSnapshot(Snapshot::Date d, const Reference &ref);


	const std::string &path() const noexcept;
	const std::vector<Snapshot *> &snapshots() noexcept;
	const std::vector<const Snapshot *> &snapshots() const noexcept;
};


}


#endif
