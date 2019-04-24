#ifndef _INCLUDE_SYNCTL_SNAPSHOT_HXX_
#define _INCLUDE_SYNCTL_SNAPSHOT_HXX_


#include <cstdint>
#include <memory>
#include <string>

#include "synctl/io/Directory.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class Snapshot
{
 public:
	using Date = uint64_t;

	struct Content
	{
		Date       date = 0;  // UNIX time of creation (0 = now)
		opcode_t   opcode;    // Opcode of the object pointed by tree
		Reference  tree;      // Root of the snapshot
		Reference  links;     // Pointer to the LinkTable_1 object
	};


 private:
	const std::string  _path;
	mutable Content    _content;
	mutable bool       _loaded = false;


	Snapshot(const std::string &path, const Snapshot::Content &content);

	void _load() const;
	void _store() const;


 public:
	Snapshot(const std::string &path);

	const Content &content() const;
	const std::string &path() const noexcept;

	static Snapshot make(const std::string &path,
			     const Snapshot::Content &content);

	static std::unique_ptr<Snapshot>
	makePtr(const std::string &path, const Snapshot::Content &content);
};


}


#endif
