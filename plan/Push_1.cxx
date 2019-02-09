#include "synctl/plan/Push_1.hxx"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <set>
#include <string>
#include <vector>

#include "synctl/io/Directory.hxx"
#include "synctl/io/NullOutputStream.hxx"
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
using synctl::NullOutputStream;
using synctl::Reference;
using synctl::Regular_1;
using synctl::Push_1;
using synctl::Symlink_1;


bool Push_1::_isReferenceKnown(const Reference &reference) const
{
	return (_knownReferences.find(reference) != _knownReferences.end());
}

void Push_1::_pushEntry(const Context *context, Reference *reference)
{
	switch (context->stat.st_mode & S_IFMT) {
	case S_IFREG:
		_pushRegular(context, reference);
		break;
	case S_IFLNK:
		_pushSymlink(context, reference);
		break;
	case S_IFDIR:
		_pushDirectory(context, reference);
		break;
	default:
		break;
	}
}

void Push_1::_pushDirectory(const Context *context, Reference *reference)
{
	Directory fsdir = Directory(context->path);
	opcode_t op = OP_TREE_DIRECTORY_1;
	NullOutputStream null;
	Directory_1 dir;
	Reference ref;
	uint64_t dlen;
	Context ctx;
	size_t plen;
	int ret;

	ctx.root = context->root;
	ctx.output = context->output;
	ctx.path = context->path;

	if (fsdir.listable() == false)
		goto write;

	plen = ctx.path.length();

	for (string &name : fsdir.trueChildren()) {
		ctx.path += '/';
		ctx.path += name;

		ret = ::lstat(ctx.path.c_str(), &ctx.stat);
		if (ret < 0)
			goto next;  // insufficient permission

		_pushEntry(&ctx, &ref);
		dir.addChild(name, ctx.stat, ref);

	 next:
		ctx.path.resize(plen);
	}

 write:
	dir.write(&null, reference);
	if (_isReferenceKnown(*reference))
		return;
	dlen = null.written();

	context->output->write(&op, sizeof (op));
	context->output->write(&dlen, sizeof (dlen));
	dir.write(context->output, nullptr);
}

void Push_1::_pushRegular(const Context *context, Reference *reference)
{
	opcode_t op = OP_TREE_REGULAR_1;
	NullOutputStream null;
	Regular_1 reg;
	Reference ref;
	uint64_t flen;

	try {
		reg = Regular_1::makeFrom(context->path);
		reg.write(&null, reference);
		if (_isReferenceKnown(*reference))
			return;
	} catch (IOException &) {
		// TODO: indicate it?
		return;
	}

	flen = context->stat.st_size;

	context->output->write(&op, sizeof (op));
	context->output->write(&flen, sizeof (flen));
	reg = Regular_1::makeFrom(context->path);
	reg.write(context->output, nullptr);
}

void Push_1::_pushSymlink(const Context *context, Reference *reference)
{
	opcode_t op = OP_TREE_SYMLINK_1;
	NullOutputStream null;
	Symlink_1 link;
	Reference ref;

	link = Symlink_1::make(context->path);

	link.write(&null, reference);
	if (_isReferenceKnown(*reference))
		return;

	context->output->write(&op, sizeof (op));
	link.write(context->output, nullptr);
}

void Push_1::addKnownReference(const Reference &reference)
{
	_knownReferences.emplace(reference);
}

void Push_1::addFilter(Filter *filter)
{
	throw 0;
}

void Push_1::push(OutputStream *output, const string &root)
{
	opcode_t op = OP_TREE_NONE;
	Reference holder;
	Context ctx;
	int ret;

	ctx.root = &root;
	ctx.output = output;
	ctx.path = root;

	ret = ::lstat(root.c_str(), &ctx.stat);
	if (ret < 0)
		throw 0;

	_pushEntry(&ctx, &holder);
	output->write(&op, sizeof (op));
}
