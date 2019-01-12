#include "synctl/SymlinkV1.hxx"

#include <cstdint>
#include <unistd.h>

#include <memory>
#include <string>

#include "synctl/HashOutputStream.hxx"
#include "synctl/InputStream.hxx"
#include "synctl/LoadContext.hxx"
#include "synctl/ReceiveContext.hxx"
#include "synctl/Reference.hxx"
#include "synctl/Repository.hxx"
#include "synctl/SendContext.hxx"
#include "synctl/StoreContext.hxx"
#include "synctl/TransientOutputStream.hxx"
#include "synctl/Type.hxx"


#define SYMLINK_PATH_BUFFER_SIZE   64


using std::string;
using std::unique_ptr;
using synctl::HashOutputStream;
using synctl::InputStream;
using synctl::LoadContext;
using synctl::ReceiveContext;
using synctl::Reference;
using synctl::Repository;
using synctl::SendContext;
using synctl::StoreContext;
using synctl::SymlinkV1;
using synctl::TransientOutputStream;
using synctl::Type;


#pragma pack(1)
struct Header
{
	uint16_t   plen;
};


static const Type __type = Type::SymlinkV1;


void SymlinkV1::recv(const string &path, ReceiveContext *ctx)
{
	string buffer, rpath;
	Header header;

	ctx->stream()->readall(&header, sizeof (header));

	buffer.resize(header.plen);
	ctx->stream()->readall(buffer.data(), header.plen);

	rpath = ctx->root() + path;
	symlink(buffer.c_str(), rpath.c_str());
}

void SymlinkV1::send(Reference *dest, const string &path, SendContext *ctx)
{
	HashOutputStream hos = HashOutputStream(ctx->stream());
	ssize_t s, l = SYMLINK_PATH_BUFFER_SIZE;
	string rpath = ctx->root() + path;
	Header header;
	string buffer;

	buffer.resize(l);

	while ((s = readlink(rpath.c_str(), buffer.data(), l)) == l) {
		l = l << 1;
		buffer.resize(l);
	}

	if (s < 0)
		return;

	hos.write(&__type, sizeof (__type));

	header.plen = (uint16_t) s;
	hos.write(&header, sizeof (header));

	hos.write(buffer.data(), header.plen);

	hos.digest(dest);
}

void SymlinkV1::load(InputStream *is, const string &path [[gnu::unused]],
		     LoadContext *ctx)
{
	Header header;
	string buffer;

	ctx->stream()->write(&__type, sizeof (__type));

	is->readall(&header, sizeof (header));
	ctx->stream()->write(&header, sizeof (header));

	buffer.resize(header.plen);
	is->readall(buffer.data(), header.plen);
	ctx->stream()->write(buffer.data(), header.plen);
}

void SymlinkV1::store(Reference *dest, StoreContext *ctx)
{
	unique_ptr<TransientOutputStream> tos = ctx->repository()->newObject();
	HashOutputStream hos = HashOutputStream(tos.get());
	Header header;
	string buffer;

	hos.write(&__type, sizeof (__type));

	ctx->stream()->readall(&header, sizeof (header));
	hos.write(&header, sizeof (header));

	buffer.resize(header.plen);
	ctx->stream()->readall(buffer.data(), header.plen);
	hos.write(buffer.data(), header.plen);

	hos.digest(dest);
	tos->commit(*dest);
}
