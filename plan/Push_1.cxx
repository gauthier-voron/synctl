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
#include "synctl/io/NullOutputStream.hxx"
#include "synctl/io/Xattribute.hxx"
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
	NullOutputStream null;
	size_t pushedCount;
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
			context->links->track(ctx.rpath, ctx.stat);
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
	dir.write(&null, reference);

	if (_isReferenceKnown(*reference))
		return true;
	dlen = null.written();

	context->output->writeInt(*opcode);
	context->output->writeInt(dlen);
	dir.write(context->output, nullptr);

	return true;
}

bool Push_1::_pushRegular(const Context *context, Reference *reference,
			  opcode_t *opcode)
{
	NullOutputStream null;
	Regular_1 reg;
	Reference ref;
	uint64_t flen;

	if (_filterPath(context) != Filter::Accept)
		return false;

	*opcode = OP_TREE_REGULAR_1;

	try {
		reg = Regular_1::makeFrom(context->apath);
		reg.write(&null, reference);
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
	reg.write(context->output, nullptr);
	_fixAccessTime(context);

	return true;
}

bool Push_1::_pushSymlink(const Context *context, Reference *reference,
			  opcode_t *opcode)
{
	NullOutputStream null;
	Symlink_1 link;
	Reference ref;

	if (_filterPath(context) != Filter::Accept)
		return false;

	*opcode = OP_TREE_SYMLINK_1;

	link = Symlink_1::make(context->apath);
	_fixAccessTime(context);

	link.write(&null, reference);
	if (_isReferenceKnown(*reference))
		return true;

	context->output->writeInt(*opcode);
	link.write(context->output, nullptr);

	return true;
}

void Push_1::_pushLinks(const Context *context, Reference *reference)
{
	opcode_t op = OP_TREE_LINK_1;
	NullOutputStream null;
	Linktable_1 table;
	Reference holder;
	uint64_t len;
	Link_1 link;

	for (const auto &lnk : context->links->getLinks()) {
		for (const auto &rpath : lnk)
			link.addLocation(rpath);

		link.write(&null, &holder);
		table.addLink(holder);

		if (_isReferenceKnown(holder) == false) {
			len = null.written();
			context->output->writeInt(op);
			context->output->writeInt(len);
			link.write(context->output);
		}

		null.reset();
		link.clear();
	}

	table.write(&null, reference);
	if (_isReferenceKnown(*reference))
		return;

	op = OP_TREE_LINKTABLE_1;
	len = null.written();

	context->output->writeInt(op);
	context->output->writeInt(len);
	table.write(context->output);
}

void Push_1::addKnownReference(const Reference &reference)
{
	_knownReferences.emplace(reference);
}

void Push_1::setFilter(Filter *filter)
{
	_filter = filter;
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

	_pushLinks(&ctx, &holder);
	output->writeInt<opcode_t>(OP_TREE_REFERENCE);

	output->write(holder.data(), holder.size());
}
