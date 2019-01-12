#include "synctl/RegularV1.hxx"

#include <sys/stat.h>
#include <sys/types.h>

#include <memory>
#include <string>

#include "synctl/FileInputStream.hxx"
#include "synctl/FileOutputStream.hxx"
#include "synctl/HashOutputStream.hxx"
#include "synctl/LoadContext.hxx"
#include "synctl/ReceiveContext.hxx"
#include "synctl/Reference.hxx"
#include "synctl/Repository.hxx"
#include "synctl/SendContext.hxx"
#include "synctl/StoreContext.hxx"
#include "synctl/TransientOutputStream.hxx"
#include "synctl/Type.hxx"


#define TRANSFER_BUFFER_SIZE  4096


using std::string;
using std::unique_ptr;
using synctl::FileInputStream;
using synctl::FileOutputStream;
using synctl::HashOutputStream;
using synctl::LoadContext;
using synctl::ReceiveContext;
using synctl::Reference;
using synctl::RegularV1;
using synctl::Repository;
using synctl::SendContext;
using synctl::StoreContext;
using synctl::TransientOutputStream;
using synctl::Type;


#pragma pack(1)
struct Header
{
	uint64_t   size;
};


static const Type __type = Type::RegularV1;


void RegularV1::_transfer(OutputStream *out, InputStream *in, size_t len)
{
	uint8_t buffer[TRANSFER_BUFFER_SIZE];
	size_t rlen, done = 0;

	while (done < len) {
		rlen = len - done;
		if (rlen > sizeof (buffer))
			rlen = sizeof (buffer);

		in->readall(buffer, rlen);
		out->write(buffer, rlen);

		done += rlen;
	}
}

void RegularV1::recv(const string &path, ReceiveContext *ctx)
{
	FileOutputStream fos;
	Header header;

	ctx->stream()->readall(&header, sizeof (header));

	fos = FileOutputStream(ctx->root() + path);
	_transfer(&fos, ctx->stream(), header.size);
}

void RegularV1::send(Reference *dest, const string &path, SendContext *ctx)
{
	HashOutputStream hos = HashOutputStream(ctx->stream());
	string rpath = ctx->root() + path;
	FileInputStream fis;
	struct stat st;
	Header header;
	int ret;

	ret = lstat(rpath.c_str(), &st);
	if (ret != 0)
		return;

	hos.write(&__type, sizeof (__type));

	header.size = st.st_size;
	hos.write(&header, sizeof (header));

	fis = FileInputStream(rpath);
	_transfer(&hos, &fis, header.size);

	hos.digest(dest);
}

void RegularV1::load(InputStream *is, const string &path [[gnu::unused]],
		     LoadContext *ctx)
{
	Header header;

	is->readall(&header, sizeof (header));

	ctx->stream()->write(&__type, sizeof (__type));
	ctx->stream()->write(&header, sizeof (header));

	_transfer(ctx->stream(), is, header.size);
}

void RegularV1::store(Reference *dest, StoreContext *ctx)
{
	unique_ptr<TransientOutputStream> tos = ctx->repository()->newObject();
	HashOutputStream hos = HashOutputStream(tos.get());
	Header header;

	hos.write(&__type, sizeof (__type));

	ctx->stream()->readall(&header, sizeof (header));
	hos.write(&header, sizeof (header));

	_transfer(&hos, ctx->stream(), header.size);

	hos.digest(dest);
	tos->commit(*dest);
}
