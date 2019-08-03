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


#ifndef _INCLUDE_SYNCTL_TRANSIENTOUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_TRANSIENTOUTPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>
#include <string>

#include "synctl/io/FileOutputStream.hxx"
#include "synctl/io/OutputStream.hxx"


namespace synctl {


class ObjectStore;
class Reference;


class TransientOutputStream : public OutputStream
{
	ObjectStore       *_ostore = nullptr;
	std::string        _path;
	FileOutputStream   _output;


 public:
	TransientOutputStream();
	TransientOutputStream(ObjectStore *ostore, const std::string &path,
			      FileOutputStream &&output);
	TransientOutputStream(const TransientOutputStream &other) = delete;
	TransientOutputStream(TransientOutputStream &&other);
	virtual ~TransientOutputStream();

	TransientOutputStream &operator=(const TransientOutputStream &other)
	        = delete;
	TransientOutputStream &operator=(TransientOutputStream &&other);

	virtual void write(uint8_t c) override;
	virtual void write(const uint8_t *src, size_t len) override;
	using OutputStream::write;

	void commit(const Reference &reference);

	virtual void flush() override;

	virtual void close() override;
};


}


#endif
