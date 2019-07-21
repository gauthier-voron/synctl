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


#include "synctl/io/LogOutputStream.hxx"

#include <cstdint>
#include <functional>
#include <string>

#include "synctl/io/OutputStream.hxx"


using std::function;
using std::string;
using synctl::LogOutputStream;
using synctl::OutputStream;


LogOutputStream::LogOutputStream(OutputStream *inner)
	: _inner(inner)
{
}

LogOutputStream::LogOutputStream(LogOutputStream &&other)
	: _inner(other._inner), _count(other._count)
{
	other._inner = nullptr;
	other._count = 0;
}

LogOutputStream &LogOutputStream::operator=(LogOutputStream &&other)
{
	_inner = other._inner;
	_count = other._count;
	other._inner = nullptr;
	other._count = 0;

	return *this;
}

void LogOutputStream::setLogger(function<void(size_t)> logger)
{
	_logger = logger;
}

size_t LogOutputStream::count() const
{
	return _count;
}

size_t LogOutputStream::reset(size_t count)
{
	size_t prev = _count;

	_count = count;

	return prev;
}

void LogOutputStream::write(uint8_t c)
{
	_inner->write(c);
	_count += 1;
	_logger(1);
}

void LogOutputStream::write(const uint8_t *src, size_t len)
{
	_inner->write(src, len);
	_count += len;
	_logger(len);
}

void LogOutputStream::write(const string &str)
{
	_inner->write(str);
	_count += str.length();
	_logger(str.length());
}

void LogOutputStream::writeStr(const string &str)
{
	_inner->writeStr(str);
	_count += str.length() + 1;
	_logger(str.length() + 1);
}

void LogOutputStream::flush()
{
	_inner->flush();
}
