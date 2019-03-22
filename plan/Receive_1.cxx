#include "synctl/plan/Receive_1.hxx"

#include <memory>

#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/LimitedInputStream.hxx"
#include "synctl/io/TransientOutputStream.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Directory_1.hxx"
#include "synctl/tree/Regular_1.hxx"
#include "synctl/tree/Symlink_1.hxx"
#include "synctl/repo/OverwriteException.hxx"
#include "synctl/repo/Repository.hxx"


using std::unique_ptr;
using synctl::Directory_1;
using synctl::HashOutputStream;
using synctl::LimitedInputStream;
using synctl::OverwriteException;
using synctl::Receive_1;
using synctl::Regular_1;
using synctl::Repository;
using synctl::Symlink_1;
using synctl::TransientOutputStream;


bool Receive_1::_receiveEntry(const Context *context)
{
	opcode_t op;

	context->input->readall(&op, sizeof (op));

	switch (op) {
	case OP_TREE_REGULAR_1:
		_receiveRegular(context);
		break;
	case OP_TREE_SYMLINK_1:
		_receiveSymlink(context);
		break;
	case OP_TREE_DIRECTORY_1:
		_receiveDirectory(context);
		break;
	case OP_TREE_REFERENCE:
		context->input->readall(context->reference->data(),
					context->reference->size());
		return false;
	default:
		throw 0;
	}

	return true;
}

void Receive_1::_receiveDirectory(const Context *context)
{
	unique_ptr<TransientOutputStream> tos;
	opcode_t op = OP_TREE_DIRECTORY_1;
	LimitedInputStream lis;
	Directory_1 dir;
	uint64_t dlen;

	context->input->readall(&dlen, sizeof (dlen));
	lis = LimitedInputStream(context->input, dlen);
	dir.read(&lis, nullptr);

	tos = context->repository->newObject();
	tos->write(&op, sizeof (op));
	dir.write(tos.get(), context->reference);

	for (Directory_1::Entry &child : dir.getChildren())
		context->repository->takeReference(child.reference);

	try {
		tos->commit(*context->reference);
	} catch (OverwriteException &e) {
		// FIXME: ignore for now
	}
}

void Receive_1::_receiveRegular(const Context *context)
{
	unique_ptr<TransientOutputStream> tos;
	opcode_t op = OP_TREE_REGULAR_1;
	LimitedInputStream lis;
	Regular_1 reg;
	uint64_t flen;

	context->input->readall(&flen, sizeof (flen));
	lis = LimitedInputStream(context->input, flen);

	tos = context->repository->newObject();
	tos->write(&op, sizeof (op));

	reg = Regular_1(tos.get());
	reg.read(&lis, context->reference);

	try {
		tos->commit(*context->reference);
	} catch (OverwriteException &e) {
		// FIXME: ignore for now
	}
}

void Receive_1::_receiveSymlink(const Context *context)
{
	unique_ptr<TransientOutputStream> tos;
	opcode_t op = OP_TREE_SYMLINK_1;
	Symlink_1 link;

	link.read(context->input, nullptr);

	tos = context->repository->newObject();
	tos->write(&op, sizeof (op));

	link.write(tos.get(), context->reference);

	try {
		tos->commit(*context->reference);
	} catch (OverwriteException &e) {
		// FIXME: ignore for now
	}
}

void Receive_1::receive(InputStream *input, Repository *repository,
			Reference *reference)
{
	Context ctx;

	ctx.input = input;
	ctx.repository = repository;
	ctx.reference = reference;

	while (_receiveEntry(&ctx))
		;
}
