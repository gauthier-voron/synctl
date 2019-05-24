#ifndef _INCLUDE_SYNCTL_TRUNK_HXX_
#define _INCLUDE_SYNCTL_TRUNK_HXX_


#include <map>
#include <memory>
#include <string>

#include "synctl/io/Directory.hxx"
#include "synctl/repo/Snapshot.hxx"


namespace synctl {


class Reference;


class Trunk
{
	Directory                                                 _dir;
	mutable std::map<std::string, std::unique_ptr<Snapshot>>  _snapshots;
	mutable std::map<std::string, Snapshot *>                 _rwptrs;
	mutable std::map<std::string, const Snapshot *>           _roptrs;
	mutable bool                                           _loaded = false;


	void _ensureLoaded() const;

	Snapshot *_newSnapshot(const std::string &name,
			       const Snapshot::Content &content) const;

	Snapshot *_newSnapshot(std::string *name,
			       const Snapshot::Content &content) const;

	Snapshot *_snapshot(const std::string &name) const;


 public:
	Trunk(const std::string &path);


	void initialize() const;
	void load() const;


	Snapshot *newSnapshot(const Snapshot::Content &content,
			      std::string *name = nullptr);

	Snapshot *snapshot(const std::string &name) noexcept;
	const Snapshot *snapshot(const std::string &name) const noexcept;

	Snapshot *lastSnapshot();
	const Snapshot *lastSnapshot() const;

	const std::map<std::string, Snapshot *> &snapshots() noexcept;
	const std::map<std::string, const Snapshot *> &snapshots()
		const noexcept;

	const std::string &path() const noexcept;
};


}


#endif
