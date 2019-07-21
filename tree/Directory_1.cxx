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


#include "synctl/tree/Directory_1.hxx"

#include <endian.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "synctl/io/EOFException.hxx"
#include "synctl/io/HashInputStream.hxx"
#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/IOException.hxx"
#include "synctl/tree/Reference.hxx"


using std::invalid_argument;
using std::make_pair;
using std::make_shared;
using std::map;
using std::out_of_range;
using std::pair;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::vector;
using synctl::Directory_1;
using synctl::EOFException;
using synctl::HashInputStream;
using synctl::HashOutputStream;
using synctl::InputStream;
using synctl::IOException;
using synctl::Reference;


Directory_1::EntryXattr::EntryXattr(const string &_name, const string &_value)
	: name(_name), value(_value)
{
}

Directory_1::EntryInfo::EntryInfo(const struct stat &_stat,
				  const map<string, string> &_xattrs,
				  opcode_t _opcode,
				  const Reference &_reference)
	: opcode(_opcode), reference(_reference)
{
	struct passwd *passwd;
	struct group *grp;

	if ((passwd = getpwuid(_stat.st_uid)) != NULL)
		user = passwd->pw_name;
	else
		user = to_string(_stat.st_uid);

	if ((grp = getgrgid(_stat.st_gid)) != NULL)
		group = grp->gr_name;
	else
		group = to_string(_stat.st_gid);

	stat.mode = htole16(_stat.st_mode);

	stat.atime  = _stat.st_atim.tv_sec;
	stat.atime *= 1000000000ul;
	stat.atime += _stat.st_atim.tv_nsec;
	stat.atime  = htole64(stat.atime);

	stat.mtime  = _stat.st_mtim.tv_sec;
	stat.mtime *= 1000000000ul;
	stat.mtime += _stat.st_mtim.tv_nsec;
	stat.mtime  = htole64(stat.mtime);

	for (auto &x : _xattrs)
		xattrs.emplace_back(x.first, x.second);
}

Directory_1::EntryInfo::EntryInfo(uint16_t _mode, uint64_t _atime,
				  uint64_t _mtime, const string &_user,
				  const string &_group,
				  const map<string, string> &_xattrs,
				  opcode_t _opcode,
				  const Reference &_reference)
	: user(_user), group(_group), opcode(_opcode), reference(_reference)
{
	stat.mode = _mode;
	stat.atime = _atime;
	stat.mtime = _mtime;

	for (auto &x : _xattrs)
		xattrs.emplace_back(x.first, x.second);
}

Directory_1::Entry::Entry(const string &name, shared_ptr<EntryInfo> &einfo)
	: _name(name), _einfo(einfo)
{
	_stat.st_mode = 0;
}

Directory_1::Entry::Entry(const string &name, shared_ptr<EntryInfo> &einfo,
			  const struct stat &stat,
			  const map<string, string> &xattrs)
	: _name(name), _einfo(einfo), _stat(stat), _xattrs(xattrs)
{
}

Directory_1::Entry::Entry()
	: _einfo(nullptr)
{
	_stat.st_mode = 0;
}

const std::string &Directory_1::Entry::name() const
{
	return _name;
}

uid_t __stid(const string &str, size_t *pos = nullptr, int base = 10)
{
	uid_t ret = ((uid_t) -1);
	size_t __pos;

	if (pos == nullptr)
		pos = &__pos;

	try {
		ret = stol(str, pos, base);
	} catch (invalid_argument &) {
		*pos = 0;
	} catch (out_of_range &) {
		*pos = 0;
	}

	if (*pos != str.length())
		throw IOException();

	return ret;
}

const struct stat &Directory_1::Entry::stat() const
{
	struct passwd *passwd;
	struct group *grp;
	uint64_t tmp;
	size_t pos;

	if (_stat.st_mode == 0) {
		_stat.st_mode = le16toh(_einfo->stat.mode);

		tmp = le64toh(_einfo->stat.atime);
		_stat.st_atim.tv_sec = tmp / 1000000000ul;
		_stat.st_atim.tv_nsec = tmp % 1000000000ul;

		tmp = le64toh(_einfo->stat.mtime);
		_stat.st_mtim.tv_sec = tmp / 1000000000ul;
		_stat.st_mtim.tv_nsec = tmp % 1000000000ul;

		if ((passwd = getpwnam(_einfo->user.c_str())) != NULL) {
			_stat.st_uid = passwd->pw_uid;
		} else {
			_stat.st_uid = __stid(_einfo->user, &pos, 10);
		}

		if ((grp = getgrnam(_einfo->group.c_str())) != NULL) {
			_stat.st_gid = grp->gr_gid;
		} else {
			_stat.st_gid = __stid(_einfo->group, &pos, 10);
		}
	}

	return _stat;
}

uint16_t Directory_1::Entry::mode() const
{
	return _einfo->stat.mode;
}

uint64_t Directory_1::Entry::atime() const
{
	return _einfo->stat.atime;
}

uint64_t Directory_1::Entry::mtime() const
{
	return _einfo->stat.mtime;
}

const string &Directory_1::Entry::user() const
{
	return _einfo->user;
}

const string &Directory_1::Entry::group() const
{
	return _einfo->group;
}

const map<string, string> &Directory_1::Entry::xattrs() const
{
	if (_xattrs.empty()) {
		for (auto &x : _einfo->xattrs)
			_xattrs[x.name] = x.value;
	}

	return _xattrs;
}

opcode_t Directory_1::Entry::opcode() const
{
	return _einfo->opcode;
}

const Reference &Directory_1::Entry::reference() const
{
	return _einfo->reference;
}

void Directory_1::_writeXattr(OutputStream *output, const EntryInfo &einfo)
	const
{
	uint64_t vlen;

	for (auto &x : einfo.xattrs) {
		vlen = x.value.size();

		output->writeStr(x.name);
		output->writeInt(vlen);
		output->write(x.value.data(), vlen);
	}

	output->writeStr("");
}

void Directory_1::_writeInfo(OutputStream *output, const EntryInfo &einfo)
	const
{
	output->write(&einfo.stat, sizeof (einfo.stat));
	output->writeStr(einfo.user);
	output->writeStr(einfo.group);
	output->writeInt(einfo.opcode);
	output->write(einfo.reference.data(), einfo.reference.size());
	_writeXattr(output, einfo);
}

void Directory_1::_readXattr(InputStream *input, EntryInfo *einfo)
{
	string name, value;
	uint64_t vlen;

	einfo->xattrs.clear();

	input->readStr(&name);

	while (name != "") {
		vlen = input->readInt<uint64_t>();

		value.resize(vlen);
		input->readall(value.data(), vlen);
		einfo->xattrs.emplace_back(name, value);

		input->readStr(&name);
	}
}

void Directory_1::_readInfo(InputStream *input, EntryInfo *einfo)
{
	input->readall(&einfo->stat, sizeof (einfo->stat));
	input->readStr(&einfo->user);
	input->readStr(&einfo->group);
	einfo->opcode = input->readInt<opcode_t>();
	input->readall(einfo->reference.data(), einfo->reference.size());
	_readXattr(input, einfo);
}

void Directory_1::_write(OutputStream *output) const
{
	for (auto it : _children) {
		const string &name = it.first;
		shared_ptr<EntryInfo> &einfo = it.second;

		output->writeStr(name);
		_writeInfo(output, *einfo);
	}
}

void Directory_1::_read(InputStream *input)
{
	shared_ptr<EntryInfo> einfo;
	string name;

	while (1) {
		try {
			input->readStr(&name);
		} catch (EOFException &) {
			break;
		}

		einfo = make_shared<EntryInfo>();
		_readInfo(input, einfo.get());
		_children[name] = einfo;
	}
}

void Directory_1::addChild(const string &name, const struct stat &statbuf,
			   const map<string, string> &xattrs, opcode_t opcode,
			   const Reference &reference)
{
	_children[name] =
		make_shared<EntryInfo>(statbuf, xattrs, opcode, reference);
}

void Directory_1::addChild(const string &name, uint16_t mode, uint64_t atime,
			   uint64_t mtime, const string &user,
			   const string &group,
			   const map<string, string> &xattrs,
			   opcode_t opcode, const Reference &reference)
{
	_children[name] =
		make_shared<EntryInfo>(mode, atime, mtime, user, group, xattrs,
				       opcode, reference);
}

void Directory_1::removeChild(const string &name)
{
	auto it = _children.find(name);

	if (it != _children.end())
		_children.erase(it);
}

vector<Directory_1::Entry> Directory_1::getChildren() const
{
	vector<Directory_1::Entry> ret;

	getChildren(&ret);

	return ret;
}

void Directory_1::getChildren(vector<Directory_1::Entry> *dest) const
{
	for (auto it : _children)
		dest->emplace_back(it.first, it.second);
}

size_t Directory_1::size() const
{
	return _children.size();
}

void Directory_1::write(OutputStream *output, Reference *ref) const
{
	HashOutputStream hos;

	if (ref == nullptr) {
		_write(output);
	} else {
		hos = HashOutputStream(output);
		_write(&hos);
		hos.digest(ref);
	}
}

void Directory_1::read(InputStream *input, Reference *ref)
{
	HashInputStream his;

	if (ref == nullptr) {
		_read(input);
	} else {
		his = HashInputStream(input);
		_read(&his);
		his.digest(ref);
	}
}
