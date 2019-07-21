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


#ifndef _INCLUDE_SYNCTL_SEND_1_HXX_
#define _INCLUDE_SYNCTL_SEND_1_HXX_


#include <cstdint>
#include <string>

#include "synctl/plan/Opcode.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/repo/Snapshot.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class InputStream;
class OutputStream;
class Repository;


class Send_1
{
	struct Context
	{
		std::string        rpath;
		opcode_t           opcode;
		Reference          reference;
		const Repository  *repository;
		Filter::Action     defact;
		OutputStream      *output;
	};


	Filter  *_filter = nullptr;


	Filter::Action _filterPath(const Context *context) const;

	void _sendObject(const Context *context);

	void _transfer(const Context *context, InputStream *input,
		       uint64_t size);

	void _sendDirectory(const Context *context, InputStream *input,
			    uint64_t size);

	void _sendLinktable(const Context *context);


 public:
	void setFilter(Filter *filter);

	void send(OutputStream *output, const Repository *repository,
		  const Snapshot::Content &content);
};


}


#endif
