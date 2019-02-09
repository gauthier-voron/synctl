#include "synctl/tree/Directory_1.hxx"

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
			  const Reference &_reference)
	: name(_name), stat(_stat), reference(_reference)
{
}

Directory_1::Entry::Entry(const string &_name, const EntryInfo &_einfo)
	: name(_name), reference(_einfo.reference)
{
	struct passwd *passwd;
	struct group *grp;
	size_t pos;

	stat.st_dev = _einfo.stat.dev;
	stat.st_ino = _einfo.stat.ino;
	stat.st_mode = _einfo.stat.mode;
	stat.st_atim.tv_sec = _einfo.stat.atime / 1000000000ul;
	stat.st_atim.tv_nsec = _einfo.stat.atime % 1000000000ul;
	stat.st_mtim.tv_sec = _einfo.stat.mtime / 1000000000ul;
	stat.st_mtim.tv_nsec = _einfo.stat.mtime % 1000000000ul;

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
}

Directory_1::EntryInfo::EntryInfo(const struct stat &_stat,
				  const Reference &_reference)
	: reference(_reference)
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

	stat.dev = _stat.st_dev;
	stat.ino = _stat.st_ino;
	stat.mode = _stat.st_mode;
	stat.atime = _stat.st_atim.tv_sec * 1000000000ul +
		_stat.st_atim.tv_nsec;
	stat.mtime = _stat.st_mtim.tv_sec * 1000000000ul +
		_stat.st_mtim.tv_nsec;
}

void Directory_1::_writeInfo(OutputStream *output, const EntryInfo &einfo)
	const
{
	output->write(&einfo.stat, sizeof (einfo.stat));
	output->writeStr(einfo.user);
	output->writeStr(einfo.group);
	output->write(einfo.reference.data(), einfo.reference.size());
}

void Directory_1::_readInfo(InputStream *input, EntryInfo *einfo)
{
	input->readall(&einfo->stat, sizeof (einfo->stat));
	input->readStr(&einfo->user);
	input->readStr(&einfo->group);
	input->readall(einfo->reference.data(), einfo->reference.size());
}

void Directory_1::_write(OutputStream *output) const
{
	for (auto it : _children) {
		const std::string &name = it.first;
		EntryInfo &einfo = it.second;

		output->write(name.c_str(), name.length() + 1);
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
			   const Reference &reference)
{
	_children[name] = EntryInfo(statbuf, reference);
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
