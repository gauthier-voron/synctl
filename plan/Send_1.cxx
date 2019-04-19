#include "synctl/plan/Send_1.hxx"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "synctl/io/InputStream.hxx"
#include "synctl/io/NullOutputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/repo/Snapshot.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/tree/Directory_1.hxx"


#define TRANSFER_BUFFER_SIZE  2097152


using std::string;
using std::unique_ptr;
using std::vector;
using synctl::Directory_1;
using synctl::Filter;
using synctl::InputStream;
using synctl::NullOutputStream;
using synctl::OutputStream;
using synctl::Send_1;
using synctl::Snapshot;


Filter::Action Send_1::_filterPath(const Context *context) const
{
	Filter::Action ret;

	if (_filter == nullptr)
		return context->defact;

	ret = _filter->apply(context->rpath);
	if (ret == Filter::Ignore)
		ret = context->defact;

	return ret;
}

void Send_1::_sendObject(const Context *context)
{
	unique_ptr<InputStream> input;
	uint64_t size;

	input = context->repository->readObject(context->reference);
	size = context->repository->getObjectSize(context->reference);

	switch (context->opcode) {
	case OP_TREE_DIRECTORY_1:
		_sendDirectory(context, input.get(), size);
		break;
	default:
		_transfer(context, input.get(), size);
		break;
	}
}

void Send_1::_transfer(const Context *context, InputStream *input,
		       uint64_t size)
{
	uint8_t buffer[TRANSFER_BUFFER_SIZE];
	size_t did;

	context->output->writeInt(size);

	do {
		did = input->read(buffer, TRANSFER_BUFFER_SIZE);
		context->output->write(buffer, did);
	} while (did > 0);
}

void Send_1::_sendDirectory(const Context *context, InputStream *input,
			    uint64_t size)
{
	NullOutputStream null;
	vector<Context> ctxs;
	bool altered = false;
	Directory_1 dir;
	Context ctx;

	dir.read(input);

	if (context->rpath[context->rpath.length() - 1] == '/')
		ctx.rpath = context->rpath;
	else
		ctx.rpath = context->rpath + '/';

	ctx.defact = context->defact;
	ctx.repository = context->repository;
	ctx.output = context->output;

	for (const Directory_1::Entry &entry : dir.getChildren()) {
		ctx.rpath += entry.name;

		if (_filterPath(&ctx) == Filter::Reject) {
			dir.removeChild(entry.name);
			altered = true;
		} else {
			ctx.opcode = entry.opcode;
			ctx.reference = entry.reference;
			ctxs.push_back(ctx);
		}

		ctx.rpath.resize(ctx.rpath.length() - entry.name.length());
	}

	if (altered) {
		dir.write(&null);
		size = null.written();
	}

	context->output->writeInt(size);
	dir.write(context->output);

	for (const Context &c : ctxs) {
		_sendObject(&c);
	}
}

void Send_1::setFilter(Filter *filter)
{
	_filter = filter;
}

void Send_1::send(OutputStream *output, const Repository *repository,
		  const Snapshot::Content &content)
{
	opcode_t op = OP_TREE_NONE;
	Context ctx;

	ctx.rpath = "/";
	ctx.defact = Filter::Accept;
	ctx.opcode = content.opcode;
	ctx.output = output;
	ctx.repository = repository;
	ctx.reference = content.tree;

	output->writeInt(ctx.opcode);
	_sendObject(&ctx);
	output->writeInt(op);
}
