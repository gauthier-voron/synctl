#ifndef _INCLUDE_SYNCTL_SERVER_SNAPSHOT_HXX_
#define _INCLUDE_SYNCTL_SERVER_SNAPSHOT_HXX_


#include <cstdint>
#include <string>

#include "synctl/Directory.hxx"
#include "synctl/Reference.hxx"


namespace synctl::server {


class Snapshot
{
 public:
	using Date = uint64_t;


 private:
	synctl::Directory          _dir;
	mutable Date               _date;
	mutable synctl::Reference  _ref;
	mutable bool               _loaded = false;


	void _ensureLoaded() const;


 public:
	Snapshot(const std::string &path);


	void initialize(const synctl::Reference &reference);
	void initialize(Date date, const synctl::Reference &reference);
	void load() const;


	Date date() const;
	const synctl::Reference &ref() const;
};


}


#endif
