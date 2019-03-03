#include "synctl/plan/Send_1.hxx"

#include <cstdint>
#include <memory>
#include <string>

#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/tree/Directory_1.hxx"


#define TRANSFER_BUFFER_SIZE  2097152


using std::string;
using std::unique_ptr;
using synctl::Directory_1;
using synctl::Filter;
using synctl::Send_1;


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
	opcode_t op;
	uint64_t size;

	input = context->repository->readObject(context->reference);
	input->readall(&op, sizeof (op));

	size = context->repository->getObjectSize(context->reference);
	size -= sizeof (op);

	context->output->write(&op, sizeof (op));

	switch (op) {
	case OP_TREE_DIRECTORY_1:
		_sendDirectory(context, input.get(), size);
		break;
	default:
		_transfer(context, input.get(), size);
		break;
	}
}

void Send_1::_transfer(const Context *context, InputStream *input, size_t size)
{
	uint8_t buffer[TRANSFER_BUFFER_SIZE];
	size_t did;

	context->output->write(&size, sizeof (size));

	do {
		did = input->read(buffer, TRANSFER_BUFFER_SIZE);
		context->output->write(buffer, did);
	} while (did > 0);
}

void Send_1::_sendDirectory(const Context *context, InputStream *input,
			    size_t size)
{
	Directory_1 dir;
	Context ctx;

	dir.read(input);

	context->output->write(&size, sizeof (size));
	dir.write(context->output);

	ctx.repository = context->repository;
	ctx.output = context->output;

	if (context->rpath[context->rpath.length() - 1] == '/')
		ctx.rpath = context->rpath;
	else
		ctx.rpath = context->rpath + '/';

	for (const Directory_1::Entry &entry : dir.getChildren()) {
		ctx.rpath += entry.name;
		ctx.reference = entry.reference;

		_sendObject(&ctx);

		ctx.rpath.resize(ctx.rpath.length() - entry.name.length());
	}
}

void Send_1::setFilter(Filter *filter)
{
	_filter = filter;
}

void Send_1::send(OutputStream *output, const Repository *repository,
		    const Reference &root)
{
	opcode_t op = OP_TREE_NONE;
	Context ctx;

	ctx.rpath = "/";
	ctx.defact = Filter::Accept;
	ctx.output = output;
	ctx.repository = repository;
	ctx.reference = root;

	_sendObject(&ctx);
	output->write(&op, sizeof (op));
}
