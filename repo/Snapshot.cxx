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


#include "synctl/repo/Snapshot.hxx"

#include <chrono>
#include <memory>
#include <string>

#include "synctl/io/FileInputStream.hxx"
#include "synctl/io/FileOutputStream.hxx"
#include "synctl/io/IOException.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Reference.hxx"


#define UINT64_STRING_MAXLEN      20
#define REFERENCE_STRING_MAXLEN   40


using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::system_clock;
using std::make_unique;
using std::stoll;
using std::string;
using std::to_string;
using std::unique_ptr;
using synctl::FileInputStream;
using synctl::FileOutputStream;
using synctl::IOException;
using synctl::Reference;
using synctl::Snapshot;


void Snapshot::_load() const
{
	FileInputStream input = FileInputStream(_path);

	_content.date = input.readInt<Date>();
	_content.opcode = input.readInt<opcode_t>();
	input.readall(_content.tree.data(), _content.tree.size());
	input.readall(_content.links.data(), _content.links.size());

	_loaded = true;
}

void Snapshot::_store() const
{
	FileOutputStream out = FileOutputStream(_path);

	out.writeInt(_content.date);
	out.writeInt(_content.opcode);
	out.write(_content.tree.data(), _content.tree.size());
	out.write(_content.links.data(), _content.links.size());

	_loaded = true;
}

Snapshot::Snapshot(const string &path, const Content &content)
	: _path(path), _content(content)
{
}

Snapshot::Snapshot(const string &path)
	: _path(path)
{
}

const Snapshot::Content &Snapshot::content() const
{
	if (_loaded == false)
		_load();
	return _content;
}

const string &Snapshot::path() const noexcept
{
	return _path;
}

Snapshot Snapshot::make(const string &path, const Content &content)
{
	Snapshot ret = Snapshot(path, content);

	if (ret._content.date == 0)
		ret._content.date = duration_cast<nanoseconds>
			(system_clock::now().time_since_epoch())
			.count();

	ret._store();
	return ret;
}

unique_ptr<Snapshot> Snapshot::makePtr(const string &path,
				       const Content &content)
{
	Snapshot *pointed = new Snapshot(path, content);
	unique_ptr<Snapshot> ret = unique_ptr<Snapshot>(pointed);

	if (ret->_content.date == 0)
		ret->_content.date = duration_cast<nanoseconds>
			(system_clock::now().time_since_epoch())
			.count();

	ret->_store();
	return ret;
}
