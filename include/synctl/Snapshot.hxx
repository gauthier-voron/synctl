#ifndef _INCLUDE_SYNCTL_SNAPSHOT_HXX_
#define _INCLUDE_SYNCTL_SNAPSHOT_HXX_


#include <cstdint>
#include <string>

#include "synctl/io/Directory.hxx"
#include "synctl/Reference.hxx"


namespace synctl {


class Snapshot
{
 public:
	using Date = uint64_t;


 private:
	Directory                  _dir;
	mutable Date               _date;
	mutable Reference          _ref;
	mutable bool               _loaded = false;


	void _ensureLoaded() const;


 public:
	Snapshot(const std::string &path);


	void initialize(const Reference &reference);
	void initialize(Date date, const Reference &reference);
	void load() const;


	Date date() const;
	const Reference &ref() const;
};


}


#endif
