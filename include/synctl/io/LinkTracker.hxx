#ifndef _INCLUDE_SYNCTL_LINKTRACKER_HXX_
#define _INCLUDE_SYNCTL_LINKTRACKER_HXX_


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <map>
#include <set>
#include <string>
#include <vector>


namespace synctl {


class LinkTracker
{
 public:
	using Entry = std::set<std::string>;


 private:
	struct Inode
	{
		dev_t  dev;
		ino_t  ino;

		Inode() = default;
		Inode(const Inode &other) = default;
		Inode(dev_t dev, ino_t ino);
		Inode(const struct stat &stat);

		bool operator<(const Inode &other) const;
	};


	std::vector<Entry>             _entries;

	std::map<Inode, size_t>        _tracked;

	std::map<std::string, size_t>  _reverse;


 public:
	LinkTracker() = default;
	LinkTracker(const LinkTracker &other) = delete;


	void track(const std::string &path, const struct stat &stat);

	void addLink(const Entry &entry);

	void getLinks(std::set<Entry> *dest) const;

	std::set<Entry> getLinks() const;

	const Entry &getLink(const std::string &path) const;
};


}


#endif
