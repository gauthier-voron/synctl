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


#ifndef _INCLUDE_SYNCTL_LOGOUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_LOGOUTPUTSTREAM_HXX_


#include <cstdint>
#include <functional>
#include <string>

#include "synctl/io/OutputStream.hxx"


namespace synctl {


class LogOutputStream : public OutputStream
{
	using Logger = std::function<void(size_t)>;


	OutputStream  *_inner = nullptr;
	size_t         _count = 0;
	Logger         _logger = [](size_t) {};


 public:
	LogOutputStream() = default;
	LogOutputStream(OutputStream *inner);
	LogOutputStream(const LogOutputStream &other) = delete;
	LogOutputStream(LogOutputStream &&other);

	LogOutputStream &operator=(const LogOutputStream &other) = delete;
	LogOutputStream &operator=(LogOutputStream &&other);

	void setLogger(std::function<void(size_t did)> logger);

	size_t count() const;
	size_t reset(size_t count = 0);

	virtual void write(uint8_t c) override;
	virtual void write(const uint8_t *src, size_t len) override;
	virtual void write(const std::string &str) override;
	virtual void writeStr(const std::string &str) override;

	virtual void flush() override;

	using OutputStream::close;
};


}


#endif
