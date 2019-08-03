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


#ifndef _INCLUDE_SYNCTL_ADAPTEROUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_ADAPTEROUTPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>
#include <ostream>

#include "synctl/io/OutputStream.hxx"


namespace synctl {


class AdapterOutputStream : public OutputStream
{
	std::ostream  *_inner = nullptr;
	bool           _attached = false;


 public:
	AdapterOutputStream();
	AdapterOutputStream(std::ostream &inner, bool attached = false);
	AdapterOutputStream(const AdapterOutputStream &other) = delete;
	AdapterOutputStream(AdapterOutputStream &&other);
	virtual ~AdapterOutputStream() = default;

	AdapterOutputStream &operator=(const AdapterOutputStream &other)
	        = delete;
	AdapterOutputStream &operator=(AdapterOutputStream &&other);

	void assign(std::ostream &inner);

	void attach();
	void attach(std::ostream &inner);

	void detach();

	virtual void write(uint8_t c) override;
	virtual void write(const uint8_t *src, size_t len) override;
	using OutputStream::write;

	virtual void flush() override;

	virtual void close() override;
};


}


#endif
