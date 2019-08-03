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


	const Entry &track(const std::string &path, const struct stat &stat);

	const Entry &addLink(const Entry &entry);

	void getLinks(std::set<Entry> *dest) const;

	std::set<Entry> getLinks() const;

	const Entry &getLink(const std::string &path) const;
};


}


#endif
