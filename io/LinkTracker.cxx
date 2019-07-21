// Copyright 2019 Gauthier Voron
//
// This file is part of Synctl
//
// Synctl is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Fundation, either version 3 of the License, or (at your option) any later
// version.
//
// Synctl is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANDABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// Synctl. If not, see <https://www.gnu.org/licenses/>.
//


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

const LinkTracker::Entry &LinkTracker::track(const string &path,
					     const struct stat &stat)
{
	size_t index, found;
	Inode inode;

	if (stat.st_nlink <= 1)
		return __empty;
	if ((stat.st_mode & S_IFMT) == S_IFDIR)
		return __empty;

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

	return _entries[index];
}

const LinkTracker::Entry &LinkTracker::addLink(const Entry &entry)
{
	size_t index;

	index = _entries.size();
	_entries.push_back(entry);

	for (const string &path : entry)
		_reverse.emplace(make_pair(path, index));

	return entry;
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
