#include "synctl/DirectoryV1.hxx"

#include <cstdint>
#include <cstring>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>

#include <filesystem>
#include <memory>
#include <string>
#include <sstream>

#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/io/Directory.hxx"
#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/LoadContext.hxx"
#include "synctl/ReceiveContext.hxx"
#include "synctl/Reference.hxx"
#include "synctl/RegularV1.hxx"
#include "synctl/Repository.hxx"
#include "synctl/SendContext.hxx"
#include "synctl/StoreContext.hxx"
#include "synctl/SymlinkV1.hxx"
#include "synctl/io/TransientOutputStream.hxx"
#include "synctl/Type.hxx"


#define TRANSFER_BUFFER_SIZE  4096


using std::ostringstream;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;
using synctl::AdapterOutputStream;
using synctl::Directory;
using synctl::DirectoryV1;
using synctl::HashOutputStream;
using synctl::InputStream;
using synctl::LoadContext;
using synctl::ReceiveContext;
using synctl::Reference;
using synctl::RegularV1;
using synctl::Repository;
using synctl::SendContext;
using synctl::StoreContext;
using synctl::SymlinkV1;
using synctl::TransientOutputStream;
using synctl::Type;


#pragma pack(1)
struct Header
{
	uint64_t   mlen;
};

#pragma pack(1)
struct ChildMeta
{
	uint8_t    dev;
	uint8_t    ino;
	uint16_t   mode;
	uint16_t   ulen;
	uint16_t   glen;
	uint16_t   nlen;
	uint64_t   atime;
	uint64_t   mtime;
};

struct ChildEntry
{
	struct stat   st;
	Reference     ref;
	string        name;
};


static const Type __type = Type::DirectoryV1;
static const Type __none = Type::None;


size_t DirectoryV1::_recvChildMeta(struct stat *st, Reference *ref,
				   string *name, ReceiveContext *ctx)
{
	string username, groupname;
	struct passwd *user;
	struct group *group;
	ChildMeta meta;
	size_t pos;

	ctx->stream()->readall(&meta, sizeof (meta));

	username.resize(meta.ulen);
	groupname.resize(meta.glen);
	name->resize(meta.nlen);

	ctx->stream()->readall(ref->data(), ref->size());
	ctx->stream()->readall(username.data(), meta.ulen);
	ctx->stream()->readall(groupname.data(), meta.glen);
	ctx->stream()->readall(name->data(), meta.nlen);

	st->st_dev = meta.dev;
	st->st_ino = meta.ino;
	st->st_mode = meta.mode;
	st->st_atim.tv_sec = meta.atime / 1000000000ul;
	st->st_atim.tv_nsec = meta.atime % 1000000000ul;
	st->st_mtim.tv_sec = meta.mtime / 1000000000ul;
	st->st_mtim.tv_nsec = meta.mtime % 1000000000ul;

	if ((user = getpwnam(username.c_str())) != NULL) {
		st->st_uid = user->pw_uid;
	} else {
		st->st_uid = stol(username, &pos, 10);
		if (pos != meta.ulen)
			st->st_uid = -1;
	}

	if ((group = getgrnam(groupname.c_str())) != NULL) {
		st->st_gid = group->gr_gid;
	} else {
		st->st_gid = stol(groupname, &pos, 10);
		if (pos != meta.glen)
			st->st_gid = -1;
	}

	return sizeof (meta) + ref->size() + meta.ulen + meta.glen + meta.nlen;
}

void DirectoryV1::_recvAddChild(const string &path, const struct stat &st,
				const Reference &ref, ReceiveContext *ctx)
{
	Type type;

	ctx->stream()->readall(&type, sizeof (type));
	
	switch (type) {
	case Type::RegularV1:
		RegularV1().recv(path, ctx);
		break;
	case Type::SymlinkV1:
		SymlinkV1().recv(path, ctx);
		break;
	case Type::DirectoryV1:
		DirectoryV1().recv(path, ctx);
		break;
	default:
		break;
	}

	_recvUpdateMeta(path, st, ctx);
}

void DirectoryV1::_recvDelChild(const string &path, ReceiveContext *ctx)
{
	std::cerr << "- " << path << std::endl;
}

void DirectoryV1::_recvUpdateChild(const string &path, const struct stat &st,
				   const Reference &ref, ReceiveContext *ctx)
{
	std::cerr << "= " << path << std::endl;
}

void DirectoryV1::_recvUpdateMeta(const string &path, const struct stat &st,
				  ReceiveContext *ctx)
{
	string repath = ctx->root() + path;
	struct timeval buf[2];
	struct stat stfs;
	int ret;

	ret = lstat(repath.c_str(), &stfs);
	if (ret != 0)
		return;     // TODO: handle error

	if ((st.st_uid != stfs.st_uid) || (st.st_gid != stfs.st_gid)) {
		ret = lchown(repath.c_str(), st.st_uid, st.st_gid);
		// TODO: handle error
	}

	if (st.st_mode != stfs.st_mode) {
		ret = chmod(repath.c_str(), (st.st_mode & ~S_IFMT));
		// TODO: handle error
	}

	if ((memcmp(&st.st_atim, &stfs.st_atim, sizeof (st.st_atim)) != 0) ||
	    (memcmp(&st.st_mtim, &stfs.st_mtim, sizeof (st.st_mtim)) != 0)) {
		buf[0].tv_sec = st.st_atim.tv_sec;
		buf[0].tv_usec = st.st_atim.tv_nsec / 1000ul;
		buf[1].tv_sec = st.st_mtim.tv_sec;
		buf[1].tv_usec = st.st_mtim.tv_nsec / 1000ul;
		ret = utimes(repath.c_str(), buf);
		// TODO: handle error
	}
}

void DirectoryV1::recv(const string &path, ReceiveContext *ctx)
{
	Directory dir = Directory(ctx->root() + path);
	vector<ChildEntry> entries;
	vector<string> fsentries;
	ChildEntry *eptr;
	size_t i, j, len;
	Header header;

	ctx->stream()->readall(&header, sizeof (header));

	len = 0;
	while (len < header.mlen) {
		entries.emplace_back();
		eptr = &entries.back();
		len += _recvChildMeta(&eptr->st, &eptr->ref, &eptr->name, ctx);
	}

	if (dir.exists() == false)
		dir.create();
	else
		fsentries = dir.sortedTrueChildren();

	i = j = 0;
	while ((i < fsentries.size()) && (j < entries.size())) {
		if (fsentries[i] < entries[j].name) {
			_recvDelChild(path + "/" + fsentries[i], ctx);
			i++;
		} else if (entries[j].name < fsentries[i]) {
			_recvAddChild(path + "/" + entries[j].name,
				      entries[j].st, entries[j].ref, ctx);
			j++;
		} else {
			_recvUpdateChild(path + "/" + entries[j].name,
					 entries[j].st, entries[j].ref, ctx);
			i++;
			j++;
		}
	}

	for (; i < fsentries.size(); i++) {
		_recvDelChild(fsentries[i], ctx);
	}

	for (; j < entries.size(); j++) {
		_recvAddChild(path + "/" + entries[j].name, entries[j].st,
			      entries[j].ref, ctx);
	}
}


size_t DirectoryV1::_loadChildMeta(vector<Reference> *refs,
				   vector<string> *paths, InputStream *is,
				   const string &path, LoadContext *ctx)
{
	string ugnames, name;
	ChildMeta meta;

	is->readall(&meta, sizeof (meta));

	ugnames.resize(meta.ulen + meta.glen);
	name.resize(meta.nlen);

	refs->emplace_back();
	is->readall(refs->back().data(), refs->back().size());
	is->readall(ugnames.data(), meta.ulen + meta.glen);
	is->readall(name.data(), meta.nlen);

	paths->push_back(path + "/" + name);

	if (ctx->contains(paths->back()) == false) {
		refs->pop_back();
		paths->pop_back();
		goto out;
	}

	ctx->stream()->write(&meta, sizeof (meta));
	ctx->stream()->write(refs->back().data(), refs->back().size());
	ctx->stream()->write(ugnames.data(), meta.ulen + meta.glen);
	ctx->stream()->write(name.data(), meta.nlen);

 out:
	return sizeof (meta) + refs->back().size() + meta.ulen +
		meta.glen + meta.nlen;
}

void DirectoryV1::_loadChild(const Reference &ref, const string &path,
			     LoadContext *ctx)
{
	unique_ptr<InputStream> eis = ctx->repository()->readObject(ref);
	Type type;

	eis->readall(&type, sizeof (type));

	switch (type) {
	case Type::RegularV1:
		RegularV1().load(eis.get(), path, ctx);
		break;
	case Type::SymlinkV1:
		SymlinkV1().load(eis.get(), path, ctx);
		break;
	case Type::DirectoryV1:
		DirectoryV1().load(eis.get(), path, ctx);
		break;
	default:
		break;
	}
}

void DirectoryV1::load(InputStream *is, const string &path, LoadContext *ctx)
{
	vector<Reference> refs;
	vector<string> paths;
	Header header;
	size_t i, len;

	ctx->stream()->write(&__type, sizeof (__type));

	is->readall(&header, sizeof (header));
	ctx->stream()->write(&header, sizeof (header));

	len = 0;
	while (len < header.mlen)
		len += _loadChildMeta(&refs, &paths, is, path, ctx);

	for (i = 0; i < refs.size(); i++)
		_loadChild(refs[i], paths[i], ctx);
}


bool DirectoryV1::_sendChild(Reference *dest, const string &path,
			     SendContext *ctx, const struct stat &st)
{
	if (ctx->contains(path) == false)
		return false;

	switch (st.st_mode & S_IFMT) {
	case S_IFREG:
		RegularV1().send(dest, path, ctx);
		return true;
	case S_IFLNK:
		SymlinkV1().send(dest, path, ctx);
		return true;
	case S_IFDIR:
		DirectoryV1().send(dest, path, ctx);
		return true;
	default:
		return false;
	}
}

size_t DirectoryV1::_sendChildMeta(OutputStream *out, const string &name,
				   const Reference &ref, const struct stat &st)
{
	string username, groupname;
	struct passwd *passwd;
	struct group *group;
	ChildMeta meta;

	if ((passwd = getpwuid(st.st_uid)) != NULL)
		username = passwd->pw_name;
	else
		username = to_string(st.st_uid);

	if ((group = getgrgid(st.st_gid)) != NULL)
		groupname = group->gr_name;
	else
		groupname = to_string(st.st_gid);

	meta.dev = st.st_dev;
	meta.ino = st.st_ino;
	meta.mode = st.st_mode;
	meta.ulen = username.length();
	meta.glen = groupname.length();
	meta.nlen = name.length();
	meta.atime = st.st_atim.tv_sec * 1000000000ul + st.st_atim.tv_nsec;
	meta.mtime = st.st_mtim.tv_sec * 1000000000ul + st.st_mtim.tv_nsec;

	out->write(&meta, sizeof (meta));
	out->write(ref.data(), ref.size());
	out->write(username.data(), meta.ulen);
	out->write(groupname.data(), meta.glen);
	out->write(name.data(), meta.nlen);

	return sizeof (meta) + ref.size() + meta.ulen + meta.glen + meta.nlen;
}

size_t DirectoryV1::_sendChildren(const string &path, SendContext *ctx,
				  OutputStream *meta)
{
	Directory dir = Directory(ctx->root() + path);
	string rpath, repath, epath;
	struct stat st;
	Reference ref;
	size_t mlen;
	int ret;

	if (dir.listable() == false)
		return 0;

	mlen = 0;
	rpath = ctx->root() + path;

	for (const string &name : dir.sortedTrueChildren()) {
		epath = path + "/" + name;
		repath = rpath + "/" + name;

		ret = lstat(repath.c_str(), &st);
		if (ret != 0)
			continue;

		if (_sendChild(&ref, epath, ctx, st) == false)
			continue;

		mlen += _sendChildMeta(meta, name, ref, st);
	}

	return mlen;
}

void DirectoryV1::send(Reference *dest, const string &path, SendContext *ctx)
{
	HashOutputStream hos = HashOutputStream(ctx->stream());
	AdapterOutputStream aos;
	ostringstream meta;
	Header header;

	aos.assign(meta);

	hos.write(&__type, sizeof (__type));

	header.mlen = _sendChildren(path, ctx, &aos);
	ctx->stream()->write(&__none, sizeof (__none));

	hos.write(&header, sizeof (header));
	hos.write(meta.str().data(), meta.str().length());
	hos.digest(dest);
}


bool DirectoryV1::_storeChild(StoreContext *ctx)
{
	Reference null;
	Type type;

	ctx->stream()->readall(&type, sizeof (type));

	switch (type) {
	case Type::RegularV1:
		RegularV1().store(&null, ctx);
		return true;
	case Type::DirectoryV1:
		DirectoryV1().store(&null, ctx);
		return true;
	case Type::SymlinkV1:
		SymlinkV1().store(&null, ctx);
		return true;
	case Type::None:
		return false;
	default:
		return false;
	}
}

void DirectoryV1::_storeChildrenMeta(Reference *dest, StoreContext *ctx)
{
	unique_ptr<TransientOutputStream> tos = ctx->repository()->newObject();
	HashOutputStream hos = HashOutputStream(tos.get());
	uint8_t buffer[TRANSFER_BUFFER_SIZE];
	size_t rlen, done = 0;
	Header header;

	hos.write(&__type, sizeof (__type));
	
	ctx->stream()->readall(&header, sizeof (header));
	hos.write(&header, sizeof (header));

	while (done < header.mlen) {
		rlen = header.mlen - done;
		if (rlen > sizeof (buffer))
			rlen = sizeof (buffer);

		ctx->stream()->readall(buffer, rlen);
		hos.write(buffer, rlen);

		done += rlen;
	}

	hos.digest(dest);
	tos->commit(*dest);
}

void DirectoryV1::store(Reference *dest, StoreContext *ctx)
{
	bool moreChildren;

	do {
		moreChildren = _storeChild(ctx);
	} while (moreChildren);

	_storeChildrenMeta(dest, ctx);
}
