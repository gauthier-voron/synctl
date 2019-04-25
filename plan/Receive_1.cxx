#include "synctl/plan/Receive_1.hxx"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/LimitedInputStream.hxx"
#include "synctl/io/Path.hxx"
#include "synctl/io/TransientOutputStream.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/repo/OverwriteException.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/tree/Directory_1.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/tree/Link_1.hxx"
#include "synctl/tree/Linktable_1.hxx"
#include "synctl/tree/Regular_1.hxx"
#include "synctl/tree/Symlink_1.hxx"


using std::make_unique;
using std::map;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::Directory_1;
using synctl::Filter;
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


Receive_1::Baseref::Baseref(const Reference &_reference, opcode_t _opcode)
	: reference(_reference), opcode(_opcode)
{
}

Receive_1::Baseref *Receive_1::_loadBasedir(MergeContext *context,
					    const string &name) const
{
	unique_ptr<Directory_1> dir = make_unique<Directory_1>();
	const Context *rc = context->rcontext;
	unique_ptr<InputStream> input;
	unique_ptr<Baseref> uptr;
	Baseref *base;
	string cpath;
	size_t cplen;

	input = rc->repository->readObject(context->baseref->reference);
	if (input == nullptr)
		return nullptr;

	context->basedir = dir.get();
	dir->read(input.get());

	if (context->path[context->path.length() - 1] == '/')
		cpath = context->path;
	else
		cpath = context->path + "/";
	cplen = cpath.length();

	for (const Directory_1::Entry &child : dir->getChildren()) {
		cpath += child.name;

		uptr = make_unique<Baseref>(child.reference, child.opcode);
		if (name == child.name)
			base = uptr.get();
		rc->baserefs->emplace(make_pair(cpath, move(uptr)));

		cpath.resize(cplen);
	}

	rc->basedirs->emplace
		(make_pair(context->baseref->reference, move(dir)));
	return base;
}

void Receive_1::_findBasedir(MergeContext *context) const
{
	const Context *rc = context->rcontext;
	auto it = rc->basedirs->find(context->baseref->reference);

	if (it != rc->basedirs->end())
		context->basedir = it->second.get();

	_loadBasedir(context, "");
}

void Receive_1::_loadRecvdir(MergeContext *context, const Reference &ref) const
{
	unique_ptr<Directory_1> dir = make_unique<Directory_1>();
	const Context *rc = context->rcontext;
	unique_ptr<InputStream> input;

	input = rc->repository->readObject(ref);

	context->recvdir = dir.get();
	dir->read(input.get());

	rc->basedirs->emplace(make_pair(ref, move(dir)));
}

void Receive_1::_findRecvdir(MergeContext *context, const Reference &ref) const
{
	const Context *rc = context->rcontext;
	auto it = rc->basedirs->find(ref);

	if (it != rc->basedirs->end()) {
		context->recvdir = it->second.get();
	} else {
		_loadRecvdir(context, ref);
	}
}

bool Receive_1::_loadBaseref(MergeContext *context) const
{
	unique_ptr<Baseref> uptr;
	MergeContext mctx;
	const Context *rc;
	string name;

	if (context->path == "/") {
		uptr = make_unique<Baseref>(_base.tree, _base.opcode);
		context->baseref = uptr.get();

		rc = context->rcontext;
		rc->baserefs->emplace(make_pair(context->path, move(uptr)));

		return true;
	}

	mctx.rcontext = context->rcontext;
	split(context->path, &mctx.path, &name);

	if (_findBaseref(&mctx) == false)
		return false;
	if (mctx.baseref->opcode != OP_TREE_DIRECTORY_1)
		return false;

	context->baseref = _loadBasedir(&mctx, name);

	return (context->baseref != nullptr);
}

bool Receive_1::_findBaseref(MergeContext *context) const
{
	auto it = context->rcontext->baserefs->find(context->path);

	if (it != context->rcontext->baserefs->end()) {
		context->baseref = it->second.get();
		return true;
	}

	return _loadBaseref(context);
}

void Receive_1::_storeDirectory(const Context *context, const Directory_1 &dir,
				const Reference &reference)
{
	unique_ptr<TransientOutputStream> tos;

	for (Directory_1::Entry &child : dir.getChildren())
		context->repository->takeReference(child.reference);

	tos = context->repository->newObject();
	dir.write(tos.get());

	try {
		tos->commit(reference);
	} catch (OverwriteException &e) {
		// May happen legally after a merge
	}
}

void Receive_1::_storeDirectory(const Context *context, const Directory_1 &dir,
				Reference *reference)
{
	unique_ptr<TransientOutputStream> tos;
	Reference ref;

	for (Directory_1::Entry &child : dir.getChildren())
		context->repository->takeReference(child.reference);

	tos = context->repository->newObject();

	if (reference == nullptr)
		reference = &ref;
	dir.write(tos.get(), reference);

	try {
		tos->commit(*reference);
	} catch (OverwriteException &e) {
		// May happen legally after a merge
	}
}

bool Receive_1::_mergeDirectory(MergeContext *context)
{
	vector<Directory_1::Entry> baseChildren, recvChildren;
	bool modified = false;
	size_t rlen, i, j;
	MergeContext mctx;
	bool hasRecv;

	_findBasedir(context);

	baseChildren = context->basedir->getChildren();
	recvChildren = context->recvdir->getChildren();

	if (context->path[context->path.length() - 1] == '/')
		mctx.path = context->path;
	else
		mctx.path = context->path + "/";
	rlen = mctx.path.length();

	mctx.rcontext = context->rcontext;

	i = j = 0;
	while (i < baseChildren.size()) {
		hasRecv = j < recvChildren.size();
		if (hasRecv && (recvChildren[j].name < baseChildren[i].name)) {
			j++;
			continue;
		}

		mctx.path += baseChildren[i].name;

		if (!hasRecv || (recvChildren[j].name > baseChildren[i].name)){
			if (_filterPath(mctx.path) == Filter::Reject) {
				context->recvdir->addChild(baseChildren[i]);
				modified = true;
			}

			i++;
			mctx.path.resize(rlen);
			continue;
		}

		if (recvChildren[j].opcode != OP_TREE_DIRECTORY_1)
			goto skip;
		if (baseChildren[i].opcode != OP_TREE_DIRECTORY_1)
			goto skip;
		if (baseChildren[i].reference == recvChildren[j].reference)
			goto skip;

		_findBaseref(&mctx);

		if (mctx.baseref->merged == false) {
			_findRecvdir(&mctx, recvChildren[j].reference);
			if (_mergeDirectory(&mctx))
				_storeDirectory(context->rcontext,
						*mctx.recvdir,
						&mctx.baseref->mergeAs);
			else
				mctx.baseref->mergeAs =
					recvChildren[j].reference;
		}

		if (mctx.baseref->mergeAs != recvChildren[j].reference) {
			context->recvdir->removeChild(recvChildren[j].name);
			recvChildren[j].reference = mctx.baseref->mergeAs;
			context->recvdir->addChild(recvChildren[j]);
			modified = true;
		}

	 skip:
		i++;
		j++;
		mctx.path.resize(rlen);
	}

	context->baseref->merged = true;

	return modified;
}


Filter::Action Receive_1::_filterPath(const string &path) const
{
	Filter::Action ret;

	ret = _filter->apply(path);
	if (ret == Filter::Ignore)
		ret = Filter::Accept;

	return ret;
}

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
	LimitedInputStream lis;
	MergeContext mctx;
	Directory_1 dir;
	uint64_t dlen;
	Reference ref;

	mctx.path = context->input->readStr();
	dlen = context->input->readInt<uint64_t>();
	lis = LimitedInputStream(context->input, dlen);
	dir.read(&lis, &ref);

	if (_filter == nullptr) {
		_storeDirectory(context, dir, ref);
		return;
	}

	mctx.rcontext = context;

	if ((_findBaseref(&mctx) == false) ||
	    (mctx.baseref->opcode != OP_TREE_DIRECTORY_1) ||
	    (mctx.baseref->reference == ref)) {
		_storeDirectory(context, dir, ref);
		return;
	}

	mctx.recvdir = &dir;

	if (_mergeDirectory(&mctx)) {
		_storeDirectory(context, dir, &mctx.baseref->mergeAs);
	} else {
		_storeDirectory(context, dir, ref);
		mctx.baseref->mergeAs = ref;
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

void Receive_1::setBaseFilter(const Snapshot::Content &base, Filter *filter)
{
	_base = base;
	_filter = filter;
}

void Receive_1::receive(InputStream *input, Repository *repository,
			Snapshot::Content *content)
{
	map<string, unique_ptr<Baseref>> baserefs;
	map<Reference, unique_ptr<Directory_1>> basedirs;
	map<Reference, unique_ptr<Directory_1>> recvdirs;
	MergeContext mctx;
	Context ctx;

	ctx.baserefs = &baserefs;
	ctx.basedirs = &basedirs;
	ctx.recvdirs = &recvdirs;
	ctx.input = input;
	ctx.repository = repository;

	while (_receiveEntry(&ctx))
		;

	content->opcode = input->readInt<opcode_t>();
	input->readall(content->tree.data(), content->tree.size());

	if ((_filter != nullptr) && (content->opcode == OP_TREE_DIRECTORY_1)) {
		mctx.path = "/";
		mctx.rcontext = &ctx;

		_findBaseref(&mctx);

		if (mctx.baseref->merged)
			content->tree = mctx.baseref->mergeAs;
	}

	while (_receiveLinks(&ctx))
		;

	input->readall(content->links.data(), content->links.size());
}
