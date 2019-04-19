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
#include <string>
#include <vector>

#include "synctl/io/EOFException.hxx"
#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/tree/Reference.hxx"


using std::make_pair;
using std::map;
using std::pair;
using std::string;
using std::to_string;
using std::vector;
using synctl::Directory_1;
using synctl::EOFException;
using synctl::HashOutputStream;
using synctl::InputStream;
using synctl::Reference;


Directory_1::Entry::Entry(const string &_name, const struct stat &_stat,
			  const map<string, string> &_xattrs, opcode_t _opcode,
			  const Reference &_reference)
	: name(_name), stat(_stat), xattrs(_xattrs), opcode(_opcode),
	  reference(_reference)
{
}

Directory_1::Entry::Entry(const string &_name, const EntryInfo &_einfo)
	: name(_name), opcode(_einfo.opcode), reference(_einfo.reference)
{
	struct passwd *passwd;
	struct group *grp;
	uint64_t tmp;
	size_t pos;

	stat.st_mode = le16toh(_einfo.stat.mode);

	tmp = le64toh(_einfo.stat.atime);
	stat.st_atim.tv_sec = tmp / 1000000000ul;
	stat.st_atim.tv_nsec = tmp % 1000000000ul;

	tmp = le64toh(_einfo.stat.mtime);
	stat.st_mtim.tv_sec = tmp / 1000000000ul;
	stat.st_mtim.tv_nsec = tmp % 1000000000ul;

	if ((passwd = getpwnam(_einfo.user.c_str())) != NULL) {
		stat.st_uid = passwd->pw_uid;
	} else {
		stat.st_uid = stol(_einfo.user, &pos, 10);
		if (pos != _einfo.user.length())
			stat.st_uid = -1;
	}

	if ((grp = getgrnam(_einfo.group.c_str())) != NULL) {
		stat.st_gid = grp->gr_gid;
	} else {
		stat.st_gid = stol(_einfo.group, &pos, 10);
		if (pos != _einfo.group.length())
			stat.st_gid = -1;
	}

	for (auto &x : _einfo.xattrs)
		xattrs[x.name] = x.value;
}

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
		EntryInfo &einfo = it.second;

		output->writeStr(name);
		_writeInfo(output, einfo);
	}
}

void Directory_1::_read(InputStream *input)
{
	EntryInfo einfo;
	string name;

	while (1) {
		try {
			input->readStr(&name);
		} catch (EOFException &) {
			break;
		}

		_readInfo(input, &einfo);
		_children[name] = einfo;
	}
}

void Directory_1::addChild(const string &name, const struct stat &statbuf,
			   const map<string, string> &xattrs, opcode_t opcode,
			   const Reference &reference)
{
	_children[name] = EntryInfo(statbuf, xattrs, opcode, reference);
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
	if (ref != nullptr)
		throw 0;  // not yet implemented
	_read(input);
}
