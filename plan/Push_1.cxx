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


#include "synctl/plan/Push_1.hxx"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <map>
#include <set>
#include <string>
#include <vector>

#include "synctl/io/Directory.hxx"
#include "synctl/io/LinkTracker.hxx"
#include "synctl/io/LogOutputStream.hxx"
#include "synctl/io/NullOutputStream.hxx"
#include "synctl/io/Xattribute.hxx"
#include "synctl/plan/LoggerPush_1.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/tree/Directory_1.hxx"
#include "synctl/tree/Link_1.hxx"
#include "synctl/tree/Linktable_1.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/tree/Regular_1.hxx"
#include "synctl/tree/Symlink_1.hxx"


using std::map;
using std::set;
using std::string;
using std::vector;
using synctl::Directory;
using synctl::Directory_1;
using synctl::Filter;
using synctl::Link_1;
using synctl::Linktable_1;
using synctl::LinkTracker;
using synctl::LoggerPush_1;
using synctl::LogOutputStream;
using synctl::NullOutputStream;
using synctl::Reference;
using synctl::Regular_1;
using synctl::Push_1;
using synctl::Symlink_1;


Push_1::EntryId::EntryId(const struct stat &stat)
	: dev(stat.st_dev), ino(stat.st_ino)
{
}

Push_1::EntryId::EntryId(EntryId &&other)
	: dev(other.dev), ino(other.ino)
{
}

bool Push_1::EntryId::operator<(const Push_1::EntryId &other) const
{
	if (dev != other.dev)
		return (dev < other.dev);
	return (ino < other.ino);
}

bool Push_1::_isReferenceKnown(const Reference &reference) const
{
	return (_knownReferences.find(reference) != _knownReferences.end());
}

Filter::Action Push_1::_filterPath(const Context *context) const
{
	Filter::Action ret;

	if (context->rpath == "/")
		return Filter::Accept;

	if (_filter == nullptr)
		return context->defact;

	ret = _filter->apply(context->rpath);
	if (ret == Filter::Ignore)
		ret = context->defact;

	return ret;
}

void Push_1::_fixAccessTime(const Context *context)
{
	string path = context->apath;
	struct timespec times[2];

	times[0] = context->stat.st_atim;
	times[1] = context->stat.st_mtim;

	utimensat(AT_FDCWD, path.c_str(), times, AT_SYMLINK_NOFOLLOW);
}

bool Push_1::_pushEntry(const Context *context, Reference *reference,
			opcode_t *opcode)
{
	switch (context->stat.st_mode & S_IFMT) {
	case S_IFREG:
		return _pushRegular(context, reference, opcode);
	case S_IFLNK:
		return _pushSymlink(context, reference, opcode);
	case S_IFDIR:
		return _pushDirectory(context, reference, opcode);
	default:
		return false;
	}
}

bool Push_1::_pushDirectory(const Context *context, Reference *reference,
			    opcode_t *opcode)
{
	Directory fsdir = Directory(context->apath);
	map<string, string> xattrs;
	map<string, string> hlinks;
	size_t pushedCount, dsize;
	NullOutputStream null;
	LogOutputStream log;
	Directory_1 dir;
	Reference ref;
	uint64_t dlen;
	Context ctx;
	opcode_t op;
	int ret;

	ctx.defact = _filterPath(context);
	if (ctx.defact == Filter::Reject)
		return false;

	if (context->rpath[context->rpath.length() - 1] == '/')
		ctx.rpath = context->rpath;
	else
		ctx.rpath = context->rpath + "/";

	ctx.apath = context->apath + "/";
	ctx.links = context->links;
	ctx.output = context->output;

	if (fsdir.listable() == false)
		goto write;

	pushedCount = 0;

	for (string &name : fsdir.trueChildren()) {
		ctx.apath += name;
		ctx.rpath += name;

		ret = ::lstat(ctx.apath.c_str(), &ctx.stat);
		if (ret < 0)
			goto next;  // insufficient permission

		try {
			xattrs = getXattributes(ctx.apath);
		} catch (IOException &) {
			// unsupported on this filesystem
		}

		if (_pushEntry(&ctx, &ref, &op)) {
			dir.addChild(name, ctx.stat, xattrs, op, ref);
			auto link = context->links->track(ctx.rpath, ctx.stat);

			if (link.size() > 1)
				for (const string &path : link)
					if (path != ctx.rpath)
						hlinks[ctx.rpath] = path;

			pushedCount += 1;
		}

	 next:
		ctx.apath.resize(ctx.apath.length() - name.length());
		ctx.rpath.resize(ctx.rpath.length() - name.length());
	}

	_fixAccessTime(context);

	if ((ctx.defact == Filter::Traverse) && (pushedCount == 0))
		return false;

 write:
	*opcode = OP_TREE_DIRECTORY_1;

	_logger->seekEntry(context->rpath, 0, 0);

	log = LogOutputStream(&null);
	log.setLogger([&](size_t did) {
		_logger->seekEntry(context->rpath, did, 0);
	});

	dir.write(&log, reference);
	dsize = log.count();

	if (hlinks.empty() == false) {
		context->output->writeInt<opcode_t>(OP_PUSH_1_LINKTRACK);
		for (const auto &pair : hlinks) {
			context->output->writeStr(pair.first);
			context->output->writeStr(pair.second);
		}
		context->output->writeStr("");
	}

	if (_isReferenceKnown(*reference))
		return true;
	dlen = null.written();

	context->output->writeInt(*opcode);
	context->output->writeStr(context->rpath);
	context->output->writeInt(dlen);

	_logger->sendEntry(context->rpath, 0, dsize);

	log = LogOutputStream(context->output);
	log.setLogger([&](size_t did) {
		_logger->sendEntry(context->rpath, did, dsize);
	});

	dir.write(&log, nullptr);

	return true;
}

bool Push_1::_pushRegular(const Context *context, Reference *reference,
			  opcode_t *opcode)
{
	NullOutputStream null;
	LogOutputStream log;
	Regular_1 reg;
	Reference ref;
	uint64_t flen;

	if (_filterPath(context) != Filter::Accept)
		return false;

	*opcode = OP_TREE_REGULAR_1;

	_logger->seekEntry(context->rpath, 0, context->stat.st_size);

	log = LogOutputStream(&null);
	log.setLogger([&](size_t did) {
		_logger->seekEntry(context->rpath, did, context->stat.st_size);
	});

	try {
		reg = Regular_1::makeFrom(context->apath);
		reg.write(&log, reference);
		if (_isReferenceKnown(*reference)) {
			_fixAccessTime(context);
			return true;
		}
	} catch (IOException &) {
		// TODO: indicate it?
		return false;
	}

	flen = context->stat.st_size;

	context->output->writeInt(*opcode);
	context->output->writeInt(flen);
	reg = Regular_1::makeFrom(context->apath);

	_logger->sendEntry(context->rpath, 0, context->stat.st_size);

	log = LogOutputStream(context->output);
	log.setLogger([&](size_t did) {
		_logger->sendEntry(context->rpath, did, context->stat.st_size);
	});

	reg.write(&log, nullptr);

	_fixAccessTime(context);

	return true;
}

bool Push_1::_pushSymlink(const Context *context, Reference *reference,
			  opcode_t *opcode)
{
	NullOutputStream null;
	Symlink_1 link;
	Reference ref;
	size_t len;

	if (_filterPath(context) != Filter::Accept)
		return false;

	*opcode = OP_TREE_SYMLINK_1;

	link = Symlink_1::make(context->apath);
	_fixAccessTime(context);

	len = link.getTarget().length();
	_logger->seekEntry(context->rpath, 0, len);
	link.write(&null, reference);
	_logger->seekEntry(context->rpath, len, len);

	if (_isReferenceKnown(*reference))
		return true;

	context->output->writeInt(*opcode);

	_logger->sendEntry(context->rpath, 0, len);
	link.write(context->output, nullptr);
	_logger->sendEntry(context->rpath, len, len);

	return true;
}

void Push_1::addKnownReference(const Reference &reference)
{
	_knownReferences.emplace(reference);
}

void Push_1::setFilter(Filter *filter)
{
	_filter = filter;
}

void Push_1::setLogger(LoggerPush_1 *logger)
{
	if (logger == nullptr)
		_logger = &_deflogger;
	else
		_logger = logger;
}

void Push_1::push(OutputStream *output, const string &root)
{
	LinkTracker links;
	Reference holder;
	Context ctx;
	opcode_t op;
	int ret;

	ctx.apath = root;
	ctx.rpath = "/";
	ctx.defact = Filter::Accept;
	ctx.links = &links;
	ctx.output = output;

	ret = ::lstat(root.c_str(), &ctx.stat);
	if (ret < 0)
		throw 0;

	_pushEntry(&ctx, &holder, &op);
	output->writeInt<opcode_t>(OP_TREE_REFERENCE);

	output->writeInt(op);
	output->write(holder.data(), holder.size());
}
