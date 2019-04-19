#include "synctl/plan/Receive_1.hxx"

#include <memory>

#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/LimitedInputStream.hxx"
#include "synctl/io/TransientOutputStream.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/repo/OverwriteException.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/tree/Directory_1.hxx"
#include "synctl/tree/Link_1.hxx"
#include "synctl/tree/Linktable_1.hxx"
#include "synctl/tree/Regular_1.hxx"
#include "synctl/tree/Symlink_1.hxx"


using std::unique_ptr;
using synctl::Directory_1;
using synctl::HashOutputStream;
using synctl::LimitedInputStream;
using synctl::Link_1;
using synctl::Linktable_1;
using synctl::OverwriteException;
using synctl::Receive_1;
using synctl::Regular_1;
using synctl::Repository;
using synctl::Symlink_1;
using synctl::TransientOutputStream;


bool Receive_1::_receiveEntry(const Context *context)
{
	opcode_t op;

	op = context->input->readInt<opcode_t>();

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
		return false;
	default:
		throw 0;
	}

	return true;
}

void Receive_1::_receiveDirectory(const Context *context)
{
	unique_ptr<TransientOutputStream> tos;
	LimitedInputStream lis;
	Reference reference;
	Directory_1 dir;
	uint64_t dlen;

	dlen = context->input->readInt<uint64_t>();
	lis = LimitedInputStream(context->input, dlen);
	dir.read(&lis, nullptr);

	tos = context->repository->newObject();
	dir.write(tos.get(), &reference);

	for (Directory_1::Entry &child : dir.getChildren())
		context->repository->takeReference(child.reference);

	try {
		tos->commit(reference);
	} catch (OverwriteException &e) {
		// FIXME: ignore for now
	}
}

void Receive_1::_receiveRegular(const Context *context)
{
	unique_ptr<TransientOutputStream> tos;
	LimitedInputStream lis;
	Reference reference;
	Regular_1 reg;
	uint64_t flen;

	flen = context->input->readInt<uint64_t>();
	lis = LimitedInputStream(context->input, flen);

	tos = context->repository->newObject();
	reg = Regular_1(tos.get());
	reg.read(&lis, &reference);

	try {
		tos->commit(reference);
	} catch (OverwriteException &e) {
		// FIXME: ignore for now
	}
}

void Receive_1::_receiveSymlink(const Context *context)
{
	unique_ptr<TransientOutputStream> tos;
	Reference reference;
	Symlink_1 link;

	link.read(context->input, nullptr);

	tos = context->repository->newObject();
	link.write(tos.get(), &reference);

	try {
		tos->commit(reference);
	} catch (OverwriteException &e) {
		// FIXME: ignore for now
	}
}

bool Receive_1::_receiveLinks(const Context *context)
{
	opcode_t op;

	op = context->input->readInt<opcode_t>();

	switch (op) {
	case OP_TREE_LINK_1:
		_receiveLink(context);
		break;
	case OP_TREE_LINKTABLE_1:
		_receiveLinktable(context);
		break;
	case OP_TREE_REFERENCE:
		return false;
	default:
		throw 0;
	}

	return true;
}

void Receive_1::_receiveLink(const Context *context)
{
	unique_ptr<TransientOutputStream> tos;
	LimitedInputStream lis;
	Reference reference;
	uint64_t len;
	Link_1 link;

	len = context->input->readInt<uint64_t>();
	lis = LimitedInputStream(context->input, len);
	link.read(&lis);

	tos = context->repository->newObject();
	link.write(tos.get(), &reference);

	try {
		tos->commit(reference);
	} catch (OverwriteException &e) {
		// FIXME: ignore for now
	}
}

void Receive_1::_receiveLinktable(const Context *context)
{
	unique_ptr<TransientOutputStream> tos;
	LimitedInputStream lis;
	Reference reference;
	Linktable_1 table;
	uint64_t len;

	len = context->input->readInt<uint64_t>();
	lis = LimitedInputStream(context->input, len);
	table.read(&lis);

	tos = context->repository->newObject();
	table.write(tos.get(), &reference);

	for (const Reference &ref : table.getLinks())
		context->repository->takeReference(ref);

	try {
		tos->commit(reference);
	} catch (OverwriteException &e) {
		// FIXME: ignore for now
	}
}

void Receive_1::receive(InputStream *input, Repository *repository,
			Snapshot::Content *content)
{
	Context ctx;

	ctx.input = input;
	ctx.repository = repository;

	while (_receiveEntry(&ctx))
		;

	content->opcode = input->readInt<opcode_t>();
	input->readall(content->tree.data(), content->tree.size());

	while (_receiveLinks(&ctx))
		;

	input->readall(content->links.data(), content->links.size());
}
