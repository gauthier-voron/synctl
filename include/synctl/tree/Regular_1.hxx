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


#ifndef _INCLUDE_SYNCTL_REGULAR_1_HXX_
#define _INCLUDE_SYNCTL_REGULAR_1_HXX_


#include <memory>

#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"


namespace synctl {


class Reference;


class Regular_1
{
	std::unique_ptr<OutputStream>   _destKeeper = nullptr;
	std::unique_ptr<InputStream>    _srcKeeper = nullptr;
	OutputStream                   *_dest = nullptr;
	InputStream                    *_src = nullptr;


	void _write(OutputStream *output) const;

	void _read(InputStream *input);


 public:
	Regular_1() = default;
	Regular_1(OutputStream *dest);
	Regular_1(std::unique_ptr<OutputStream> dest);
	Regular_1(InputStream *src);
	Regular_1(std::unique_ptr<InputStream> src);
	Regular_1(InputStream *src, OutputStream *dest);
	Regular_1(std::unique_ptr<InputStream> src,
		  std::unique_ptr<OutputStream> dest);
	Regular_1(Regular_1 &&other) = default;

	Regular_1 &operator=(Regular_1 &&other) = default;


	void setOutput(OutputStream *dest);
	void setOutput(std::unique_ptr<OutputStream> dest);

	void setInput(InputStream *src);
	void setInput(std::unique_ptr<InputStream> dest);


	void write(OutputStream *output, Reference *ref = nullptr) const;

	void read(InputStream *input, Reference *ref = nullptr);


	static Regular_1 make(const std::string &path);
	static Regular_1 makeTo(const std::string &path);
	static Regular_1 makeFrom(const std::string &path);
};


}


#endif
