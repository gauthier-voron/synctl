#include "synctl/plan/Push_1.hxx"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>
#include <string>
#include <vector>

#include "synctl/io/Directory.hxx"
#include "synctl/io/NullOutputStream.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Directory_1.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/tree/Regular_1.hxx"
#include "synctl/tree/Symlink_1.hxx"


using std::set;
using std::string;
using std::vector;
using synctl::Directory;
using synctl::Directory_1;
using synctl::Filter;
using synctl::NullOutputStream;
using synctl::Reference;
using synctl::Regular_1;
using synctl::Push_1;
using synctl::Symlink_1;


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

bool Push_1::_pushEntry(const Context *context, Reference *reference)
{
	switch (context->stat.st_mode & S_IFMT) {
	case S_IFREG:
		return _pushRegular(context, reference);
	case S_IFLNK:
		return _pushSymlink(context, reference);
	case S_IFDIR:
		return _pushDirectory(context, reference);
	default:
		return false;
	}
}

bool Push_1::_pushDirectory(const Context *context, Reference *reference)
{
	Directory fsdir = Directory(context->apath);
	opcode_t op = OP_TREE_DIRECTORY_1;
	NullOutputStream null;
	size_t pushedCount;
	Directory_1 dir;
	Reference ref;
	uint64_t dlen;
	Context ctx;
	int ret;

	ctx.defact = _filterPath(context);
	if (ctx.defact == Filter::Reject)
		return false;

	if (context->rpath[context->rpath.length() - 1] == '/')
		ctx.rpath = context->rpath;
	else
		ctx.rpath = context->rpath + "/";

	ctx.apath = context->apath + "/";
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

		if (_pushEntry(&ctx, &ref)) {
			dir.addChild(name, ctx.stat, ref);
			pushedCount += 1;
		}

	 next:
		ctx.apath.resize(ctx.apath.length() - name.length());
		ctx.rpath.resize(ctx.rpath.length() - name.length());
	}

	if ((ctx.defact == Filter::Traverse) && (pushedCount == 0))
		return false;

 write:
	dir.write(&null, reference);
	if (_isReferenceKnown(*reference))
		return true;
	dlen = null.written();

	context->output->writeInt(op);
	context->output->writeInt(dlen);
	dir.write(context->output, nullptr);

	return true;
}

bool Push_1::_pushRegular(const Context *context, Reference *reference)
{
	opcode_t op = OP_TREE_REGULAR_1;
	NullOutputStream null;
	Regular_1 reg;
	Reference ref;
	uint64_t flen;

	if (_filterPath(context) != Filter::Accept)
		return false;

	try {
		reg = Regular_1::makeFrom(context->apath);
		reg.write(&null, reference);
		if (_isReferenceKnown(*reference))
			return true;
	} catch (IOException &) {
		// TODO: indicate it?
		return false;
	}

	flen = context->stat.st_size;

	context->output->writeInt(op);
	context->output->writeInt(flen);
	reg = Regular_1::makeFrom(context->apath);
	reg.write(context->output, nullptr);

	return true;
}

bool Push_1::_pushSymlink(const Context *context, Reference *reference)
{
	opcode_t op = OP_TREE_SYMLINK_1;
	NullOutputStream null;
	Symlink_1 link;
	Reference ref;

	if (_filterPath(context) != Filter::Accept)
		return false;

	link = Symlink_1::make(context->apath);

	link.write(&null, reference);
	if (_isReferenceKnown(*reference))
		return true;

	context->output->writeInt(op);
	link.write(context->output, nullptr);

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

void Push_1::push(OutputStream *output, const string &root)
{
	Reference holder;
	Context ctx;
	opcode_t op;
	int ret;

	ctx.apath = root;
	ctx.rpath = "/";
	ctx.defact = Filter::Accept;
	ctx.output = output;

	ret = ::lstat(root.c_str(), &ctx.stat);
	if (ret < 0)
		throw 0;

	_pushEntry(&ctx, &holder);

	op = OP_TREE_REFERENCE;
	output->writeInt(op);
	output->write(holder.data(), holder.size());
}
