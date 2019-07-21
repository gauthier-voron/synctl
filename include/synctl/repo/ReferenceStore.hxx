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


#ifndef _INCLUDE_SYNCTL_REFERENCESTORE_HXX_
#define _INCLUDE_SYNCTL_REFERENCESTORE_HXX_


#include <cstdint>
#include <map>
#include <string>

#include "synctl/io/Directory.hxx"


namespace synctl {


class OutputStream;
class Reference;


using Refcount = uint32_t;


class ReferenceStore
{
	Directory                              _dir;
	mutable std::map<Reference, Refcount>  _refcounts;
	mutable bool                           _loaded = false;


	void _load() const;

	void _store() const;

	void _ensureLoaded() const;


 public:
	ReferenceStore(const std::string &path);
	~ReferenceStore();


	void initialize() const;


	Refcount readRefcount(const Reference &reference) const;

	void writeRefcount(const Reference &reference, Refcount cnt);


	void takeReference(const Reference &reference);

	void dumpReferences(OutputStream *output) const;
};


}


#endif
