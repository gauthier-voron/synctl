#include "synctl/plan/Receive_1.hxx"

#include <memory>
#include <string>

#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/LimitedInputStream.hxx"
#include "synctl/io/Path.hxx"
#include "synctl/io/TransientOutputStream.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/plan/SnapshotCombiner.hxx"
#include "synctl/repo/OverwriteException.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/tree/Directory_1.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/tree/Link_1.hxx"
#include "synctl/tree/Linktable_1.hxx"
#include "synctl/tree/Regular_1.hxx"
#include "synctl/tree/Symlink_1.hxx"


using std::string;
using std::unique_ptr;
using synctl::Directory_1;
using synctl::Filter;
using synctl::HashOutputStream;
using synctl::LimitedInputStream;
using synctl::OverwriteException;
using synctl::Receive_1;
using synctl::Regular_1;
using synctl::Repository;
using synctl::SnapshotCombiner;
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
	Reference reference, *dest;
	LimitedInputStream lis;
	Directory_1 dir;
	uint64_t dlen;
	string path;

	path = context->input->readStr();
	dlen = context->input->readInt<uint64_t>();
	lis = LimitedInputStream(context->input, dlen);
	dir.read(&lis, &reference);

	dest = nullptr;
	if (context->scombiner != nullptr) {
		if (context->scombiner->merge(path, reference, &dir))
			dest = &reference;
	}

	for (Directory_1::Entry &child : dir.getChildren())
		context->repository->takeReference(child.reference);

	tos = context->repository->newObject();
	dir.write(tos.get(), dest);

	try {
		tos->commit(reference);
	} catch (OverwriteException &e) {
		// May happen legally after a merge
	}

	if (context->scombiner != nullptr)
		context->scombiner->assign(path, reference);
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

void Receive_1::setBaseFilter(const Snapshot::Content &base, Filter *filter)
{
	_base = base;
	_filter = filter;
}

void Receive_1::receive(InputStream *input, Repository *repository,
			Snapshot::Content *content)
{
	SnapshotCombiner scombiner;
	Context ctx;

	ctx.input = input;
	ctx.repository = repository;

	if (_filter != nullptr) {
		scombiner = SnapshotCombiner(repository, _filter, _base);
		ctx.scombiner = &scombiner;
	} else {
		ctx.scombiner = nullptr;
	}

	while (_receiveEntry(&ctx))
		;

	content->opcode = input->readInt<opcode_t>();
	input->readall(content->tree.data(), content->tree.size());

	if (ctx.scombiner != nullptr)
		ctx.scombiner->merged("/", &content->tree);

	while (_receiveLinks(&ctx))
		;

	input->readall(content->links.data(), content->links.size());
}
