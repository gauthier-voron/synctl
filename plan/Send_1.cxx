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
#include "synctl/tree/Linktable_1.hxx"


#define TRANSFER_BUFFER_SIZE  2097152


using std::string;
using std::unique_ptr;
using std::vector;
using synctl::Directory_1;
using synctl::Filter;
using synctl::InputStream;
using synctl::Linktable_1;
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
	vector<uint8_t> buffer = vector<uint8_t>(TRANSFER_BUFFER_SIZE);
	size_t did;

	context->output->writeInt(size);

	do {
		did = input->read(buffer.data(), TRANSFER_BUFFER_SIZE);
		context->output->write(buffer.data(), did);
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
		ctx.rpath += entry.name();

		if (_filterPath(&ctx) == Filter::Reject) {
			dir.removeChild(entry.name());
			altered = true;
		} else {
			ctx.opcode = entry.opcode();
			ctx.reference = entry.reference();
			ctxs.push_back(ctx);
		}

		ctx.rpath.resize(ctx.rpath.length() - entry.name().length());
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

void Send_1::_sendLinktable(const Context *context)
{
	unique_ptr<InputStream> input;
	Linktable_1 table;
	uint64_t size;
	Context ctx;

	input = context->repository->readObject(context->reference);

	table.read(input.get());

	size = table.size();
	context->output->writeInt(size);

	ctx.repository = context->repository;
	ctx.defact = context->defact;
	ctx.output = context->output;
	ctx.opcode = OP_TREE_LINK_1;

	for (const Reference &link : table.getLinks()) {
		ctx.reference = link;
		_sendObject(&ctx);
	}
}

void Send_1::setFilter(Filter *filter)
{
	_filter = filter;
}

void Send_1::send(OutputStream *output, const Repository *repository,
		  const Snapshot::Content &content)
{
	Context ctx;

	ctx.output = output;
	ctx.repository = repository;

	ctx.defact = Filter::Accept;
	ctx.opcode = OP_TREE_LINKTABLE_1;
	ctx.reference = content.links;

	_sendLinktable(&ctx);

	ctx.rpath = "/";
	ctx.defact = Filter::Accept;
	ctx.opcode = content.opcode;
	ctx.reference = content.tree;

	output->writeInt(ctx.opcode);
	_sendObject(&ctx);
}
