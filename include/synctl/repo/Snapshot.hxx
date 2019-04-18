#ifndef _INCLUDE_SYNCTL_SNAPSHOT_HXX_
#define _INCLUDE_SYNCTL_SNAPSHOT_HXX_


#include <cstdint>
#include <string>

#include "synctl/io/Directory.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class Snapshot
{
 public:
	using Date = uint64_t;

	struct Content
	{
		Date       date = 0;  // UNIX time of creation (0 = now)
		Reference  tree;      // Root of the snapshot
	};


 private:
	Directory        _dir;
	mutable Content  _content;
	mutable bool     _loaded = false;


	void _store() const;
	void _ensureLoaded() const;


 public:
	Snapshot(const std::string &path);

	void initialize(const Content &content);
	void load() const;

	const Content &content() const;
	const std::string &path() const noexcept;
};


}


#endif
