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


#include "synctl/repo/Branch.hxx"

#include <string>

#include "synctl/io/FileInputStream.hxx"
#include "synctl/io/FileOutputStream.hxx"


using std::string;
using synctl::Branch;
using synctl::FileInputStream;
using synctl::FileOutputStream;


Branch::Branch(const string &path)
	: _path(path)
{
}

void Branch::store(const Content &content)
{
	FileOutputStream out = FileOutputStream(_path);

	out.writeStr(content.trunk);
	out.writeStr(content.snapshot);

	_loaded = true;
}

void Branch::load() const
{
	FileInputStream input = FileInputStream(_path);

	_content.trunk = input.readStr();
	_content.snapshot = input.readStr();

	_loaded = true;
}

const Branch::Content &Branch::content() const
{
	if (_loaded == false)
		load();

	return _content;
}

const string &Branch::path() const noexcept
{
	return _path;
}
