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


#ifndef _INCLUDE_SYNCTL_RECEIVE_1_HXX_
#define _INCLUDE_SYNCTL_RECEIVE_1_HXX_


#include <map>
#include <memory>
#include <string>

#include "synctl/plan/LinkBuilder.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/repo/Snapshot.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class Directory_1;
class InputStream;
class Link_1;
class Repository;
class SnapshotCombiner;


class Receive_1
{
	struct Context
	{
		SnapshotCombiner                                   *scombiner;
		LinkBuilder                                        *lbuilder;
		InputStream                                        *input;
		Repository                                         *repository;
	};


	Snapshot::Content   _base;
	Filter             *_filter = nullptr;


	bool _receiveEntry(const Context *context);

	void _receiveDirectory(const Context *context);

	void _receiveRegular(const Context *context);

	void _receiveSymlink(const Context *context);

	void _receiveLinktrack(const Context *context);


 public:
	void setBaseFilter(const Snapshot::Content &base, Filter *filter);

	void receive(InputStream *input, Repository *repository,
		     Snapshot::Content *content);
};


}


#endif
