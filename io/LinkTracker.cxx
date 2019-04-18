#include "synctl/io/LinkTracker.hxx"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <map>
#include <set>
#include <string>
#include <vector>


using std::make_pair;
using std::map;
using std::set;
using std::string;
using std::vector;
using synctl::LinkTracker;


static LinkTracker::Entry __empty;


LinkTracker::Inode::Inode(dev_t _dev, ino_t _ino)
	: dev(_dev), ino(_ino)
{
}

LinkTracker::Inode::Inode(const struct stat &stat)
	: dev(stat.st_dev), ino(stat.st_ino)
{
}

bool LinkTracker::Inode::operator<(const Inode &other) const
{
	if (dev != other.dev)
		return (dev < other.dev);
	return (ino < other.ino);
}

void LinkTracker::track(const string &path, const struct stat &stat)
{
	size_t index, found;
	Inode inode;

	if (stat.st_nlink <= 1)
		return;
	if ((stat.st_mode & S_IFMT) == S_IFDIR)
		return;

	inode.dev = stat.st_dev;
	inode.ino = stat.st_ino;

	index = _entries.size();
	found = _tracked.emplace(make_pair(inode, index)).first->second;

	if (found == index)
		_entries.emplace_back();
	else
		index = found;

	_entries[index].insert(path);
	_reverse.emplace(make_pair(path, index));
}

void LinkTracker::addLink(const Entry &entry)
{
	size_t index;

	index = _entries.size();
	_entries.push_back(entry);

	for (const string &path : entry)
		_reverse.emplace(make_pair(path, index));
}

void LinkTracker::getLinks(set<Entry> *dest) const
{
	for (const Entry &entry : _entries) {
		if (entry.size() <= 1)
			continue;
		dest->insert(entry);
	}
}

set<LinkTracker::Entry> LinkTracker::getLinks() const
{
	set<Entry> ret;

	getLinks(&ret);
	return ret;
}

const LinkTracker::Entry &LinkTracker::getLink(const string &path) const
{
	auto it = _reverse.find(path);

	if (it == _reverse.end())
		return __empty;

	return _entries[it->second];
}
