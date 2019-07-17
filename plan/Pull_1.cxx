#include "synctl/plan/Pull_1.hxx"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>
#include <string>
#include <vector>

#include "synctl/io/Directory.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/IOException.hxx"
#include "synctl/io/LimitedInputStream.hxx"
#include "synctl/io/LinkTracker.hxx"
#include "synctl/io/Symlink.hxx"
#include "synctl/io/Xattribute.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Directory_1.hxx"
#include "synctl/tree/Link_1.hxx"
#include "synctl/tree/Regular_1.hxx"
#include "synctl/tree/Symlink_1.hxx"


#define CONSUME_BUFFER_SIZE  16384


using std::set;
using std::string;
using std::vector;
using synctl::Directory;
using synctl::Directory_1;
using synctl::InputStream;
using synctl::IOException;
using synctl::LimitedInputStream;
using synctl::Link_1;
using synctl::LinkTracker;
using synctl::Pull_1;
using synctl::Regular_1;
using synctl::Symlink;
using synctl::Symlink_1;


void __applyStat(const std::string &path, const struct stat &stat)
{
	struct timespec times[2];
	struct stat st;

	if (::lstat(path.c_str(), &st) != 0)
		throw IOException();

	if (st.st_mode != stat.st_mode)
		if (::chmod(path.c_str(), stat.st_mode) != 0)
			throw IOException();

	if ((st.st_uid != stat.st_uid) || (st.st_gid != stat.st_gid))
		if (::chown(path.c_str(), stat.st_uid, stat.st_gid) != 0)
			throw IOException();

	times[0] = stat.st_atim;
	times[1] = stat.st_mtim;
	if (utimensat(AT_FDCWD, path.c_str(), times, AT_SYMLINK_NOFOLLOW) != 0)
		throw IOException();
}

void Pull_1::_delete(const std::string &path)
{
	Directory directory = Directory(path);
	int ret;

	if (directory.exists()) {
		directory.remove();
	} else {
		ret = unlink(path.c_str());
		if (ret != 0)
			throw IOException(path);
	}
}

void Pull_1::_delete(const Context *context)
{
	_delete(context->apath);
}

bool Pull_1::_linkLocally(const Context *context)
{
	LinkTracker::Entry entry = context->tracker->getLink(context->rpath);
	size_t alen, rlen;
	string alink;
	int ret;

	if (entry.size() <= 1)
		return false;

	for (const string &other : entry) {
		if (context->rdone->find(other) == context->rdone->end())
			continue;

		alen = context->apath.length();
		rlen = context->rpath.length();
		alink = context->apath.substr(0, alen - rlen);
		alink += other;

		::unlink(context->apath.c_str());
		ret = ::link(alink.c_str(), context->apath.c_str());
		if (ret != 0) {
			perror("");
			throw 0;
		}

		return true;
	}

	return false;
}

void Pull_1::_pullObject(const Context *context)
{
	switch (context->opcode) {
	case OP_TREE_NONE:
		break;
	case OP_TREE_DIRECTORY_1:
		_pullDirectory(context);
		break;
	case OP_TREE_REGULAR_1:
		_pullRegular(context);
		break;
	case OP_TREE_SYMLINK_1:
		_pullSymlink(context);
		break;
	default:
		throw 0;
	}
}

void Pull_1::_createDirectory(const Context *context)
{
	Directory directory = Directory(context->apath);
	LimitedInputStream lis;
	Directory_1 dir;
	uint64_t size;
	Context ctx;

	size = context->input->readInt<uint64_t>();
	lis = LimitedInputStream(context->input, size);
	dir.read(&lis);

	ctx.apath = context->apath + '/';
	ctx.rdone = context->rdone;
	ctx.input = context->input;
	ctx.tracker = context->tracker;

	if (context->rpath[context->rpath.length() - 1] == '/')
		ctx.rpath = context->rpath;
	else
		ctx.rpath = context->rpath + "/";

	directory.create();

	for (const Directory_1::Entry &entry : dir.getChildren()) {
		ctx.apath += entry.name();
		ctx.rpath += entry.name();
		ctx.opcode = entry.opcode();

		_pullObject(&ctx);
		__applyStat(ctx.apath, entry.stat());
		setXattributes(ctx.apath, entry.xattrs());

		ctx.apath.resize(ctx.apath.length() - entry.name().length());
		ctx.rpath.resize(ctx.rpath.length() - entry.name().length());
	}
}

void Pull_1::_mergeDirectory(const Context *context)
{
	Directory directory = Directory(context->apath);
	vector<Directory_1::Entry> remoteChildren;
	vector<string> localChildren;
	LimitedInputStream lis;
	Directory_1 dir;
	uint64_t size;
	bool hasLocal;
	size_t i, j;
	Context ctx;

	size = context->input->readInt<uint64_t>();
	lis = LimitedInputStream(context->input, size);
	dir.read(&lis);

	localChildren = directory.sortedTrueChildren();
	i = 0;

	remoteChildren = dir.getChildren();
	j = 0;

	ctx.apath = context->apath + '/';
	ctx.rdone = context->rdone;
	ctx.input = context->input;
	ctx.tracker = context->tracker;

	if (context->rpath[context->rpath.length() - 1] == '/')
		ctx.rpath = context->rpath;
	else
		ctx.rpath = context->rpath + "/";

	while (j < remoteChildren.size()) {
		hasLocal = i < localChildren.size();
		if (hasLocal && (localChildren[i] < remoteChildren[j].name())){
			_delete(ctx.apath + localChildren[i]);
			i++;
		}

		ctx.apath += remoteChildren[j].name();
		ctx.rpath += remoteChildren[j].name();
		ctx.opcode = remoteChildren[j].opcode();

		_pullObject(&ctx);
		setXattributes(ctx.apath, remoteChildren[j].xattrs());
		__applyStat(ctx.apath, remoteChildren[j].stat());

		ctx.apath.resize(ctx.apath.length() -
				 remoteChildren[j].name().length());
		ctx.rpath.resize(ctx.rpath.length() -
				 remoteChildren[j].name().length());

		if (hasLocal && (localChildren[i] == remoteChildren[j].name()))
			i++;
		j++;
	}

	while (i < localChildren.size()) {
		_delete(ctx.apath + localChildren[i]);
		i++;
	}
}

void Pull_1::_pullDirectory(const Context *context)
{
	struct stat st;
	int ret;

	ret = ::lstat(context->apath.c_str(), &st);
	if (ret != 0) {
		_createDirectory(context);
	} else if ((st.st_mode & S_IFMT) == S_IFDIR) {
		_mergeDirectory(context);
	} else {
		_delete(context);
		_createDirectory(context);
	}

	context->rdone->insert(context->rpath);
}

void Pull_1::_pullRegular(const Context *context)
{
	uint8_t buffer[CONSUME_BUFFER_SIZE];
	LimitedInputStream lis;
	struct stat st;
	Regular_1 reg;
	uint64_t size;
	size_t did;
	int ret;

	ret = ::lstat(context->apath.c_str(), &st);
	if ((ret == 0) && ((st.st_mode & S_IFMT) != S_IFREG))
		_delete(context);

	size = context->input->readInt<uint64_t>();
	lis = LimitedInputStream(context->input, size);

	if (_linkLocally(context)) {
		// a better Send_X version would not even send this data.
		do {
			did = lis.read(buffer, CONSUME_BUFFER_SIZE);
		} while (did > 0);
	} else {
		reg = Regular_1::makeTo(context->apath);
		reg.read(&lis);
	}

	context->rdone->insert(context->rpath);
}

void Pull_1::_pullSymlink(const Context *context)
{
	struct stat st;
	Symlink_1 link;
	int ret;

	ret = ::lstat(context->apath.c_str(), &st);
	if ((ret == 0) && ((st.st_mode & S_IFMT) != S_IFLNK))
		_delete(context);

	// discard size since we stop on asciiz end
	context->input->readInt<uint64_t>();

	link.read(context->input);

	if (_linkLocally(context) == false)
		link.apply(context->apath);

	context->rdone->insert(context->rpath);
}

void Pull_1::_pullLinks(const Context *context)
{
	LinkTracker::Entry entry;
	uint64_t i, count, size;
	LimitedInputStream lis;
	Link_1 link;

	count = context->input->readInt<uint64_t>();

	for (i = 0; i < count; i++) {
		size = context->input->readInt<uint64_t>();
		lis = LimitedInputStream(context->input, size);
		link.read(&lis);

		for (const string &path : link.getLocations())
			entry.insert(path);

		context->tracker->addLink(entry);
		entry.clear();
		link.clear();
	}
}

void Pull_1::pull(InputStream *input, const string &root)
{
	LinkTracker tracker;
	set<string> rdone;
	Context ctx;

	ctx.apath = root;
	ctx.rpath = "/";
	ctx.rdone = &rdone;
	ctx.input = input;
	ctx.tracker = &tracker;

	_pullLinks(&ctx);

	ctx.opcode = input->readInt<opcode_t>();
	_pullObject(&ctx);
}
