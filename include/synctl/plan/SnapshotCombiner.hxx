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


#ifndef _INCLUDE_SYNCTL_SNAPSHOTCOMBINER_HXX_
#define _INCLUDE_SYNCTL_SNAPSHOTCOMBINER_HXX_


#include <memory>
#include <string>

#include "synctl/plan/Explorer.hxx"
#include "synctl/repo/Snapshot.hxx"
#include "synctl/tree/Directory_1.hxx"
#include "synctl/tree/Filter.hxx"


namespace synctl {


class Reference;


class SnapshotCombiner
{
	Repository                        *_repository;
	Filter                            *_filter;
	Snapshot::Content                  _base;
	Explorer                          *_explorer;
	std::unique_ptr<Explorer>          _privateExplorer;
	std::vector<std::string>           _importedPaths;
	std::map<std::string, Reference>   _mergedPaths;


	Filter::Action _filterPath(const std::string &path) const;

	void _import(const std::string &name, Directory_1 *received,
		     const Explorer::Entry &base);

	void _store(const Directory_1 &object, Reference *reference = nullptr);

	bool _combine(const std::string &path, Directory_1 *received,
		      const Directory_1::Entry &recv,
		      const Explorer::Entry &base);

	bool _merge(const std::string &path, Directory_1 *received,
		    const Explorer::Entry &base);


 public:
	SnapshotCombiner() = default;
	SnapshotCombiner(Repository *repository, Filter *filter,
			 const Snapshot::Content &base,
			 Explorer *explorer = nullptr);
	SnapshotCombiner(SnapshotCombiner &&other);

	SnapshotCombiner &operator=(SnapshotCombiner &&other);


	bool merge(const std::string &path, const Reference &reference,
		   Directory_1 *received);

	void assign(const std::string &path, const Reference &reference);


	bool merged(const std::string &path, Reference *reference = nullptr)
		const;

	const std::vector<std::string> &importedPaths() const;
};


}


#endif
