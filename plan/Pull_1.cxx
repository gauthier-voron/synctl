#include "synctl/plan/Pull_1.hxx"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "synctl/io/Directory.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/IOException.hxx"
#include "synctl/io/LimitedInputStream.hxx"
#include "synctl/io/Symlink.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Directory_1.hxx"
#include "synctl/tree/Regular_1.hxx"
#include "synctl/tree/Symlink_1.hxx"


using std::string;
using synctl::Directory;
using synctl::Directory_1;
using synctl::InputStream;
using synctl::IOException;
using synctl::LimitedInputStream;
using synctl::Pull_1;
using synctl::Regular_1;
using synctl::Symlink;
using synctl::Symlink_1;


void __applyStat(const std::string &path, const struct stat &stat)
{
	struct timespec times[2];
	struct stat st;

	if (::lstat(path.c_str(), &st) != 0)
		throw IOException();

	if (st.st_mode != stat.st_mode)
		if (::chmod(path.c_str(), stat.st_mode) != 0)
			throw IOException();

	if ((st.st_uid != stat.st_uid) || (st.st_gid != stat.st_gid))
		if (::chown(path.c_str(), stat.st_uid, stat.st_gid) != 0)
			throw IOException();

	times[0] = stat.st_atim;
	times[1] = stat.st_mtim;
	if (utimensat(AT_FDCWD, path.c_str(), times, AT_SYMLINK_NOFOLLOW) != 0)
		throw IOException();
}

void Pull_1::_delete(const std::string &path)
{
	Directory directory = Directory(path);
	int ret;

	if (directory.exists()) {
		directory.remove();
	} else {
		ret = unlink(path.c_str());
		if (ret != 0)
			throw IOException(path);
	}
}

void Pull_1::_delete(const Context *context)
{
	_delete(context->apath);
}

void Pull_1::_pullObject(const Context *context)
{
	opcode_t op;

	context->input->readall(&op, sizeof (op));

	switch (op) {
	case OP_TREE_NONE:
		break;
	case OP_TREE_DIRECTORY_1:
		_pullDirectory(context);
		break;
	case OP_TREE_REGULAR_1:
		_pullRegular(context);
		break;
	case OP_TREE_SYMLINK_1:
		_pullSymlink(context);
		break;
	default:
		throw 0;
	}
}

void Pull_1::_createDirectory(const Context *context)
{
	Directory directory = Directory(context->apath);
	LimitedInputStream lis;
	Directory_1 dir;
	uint64_t size;
	Context ctx;

	context->input->readall(&size, sizeof (size));
	lis = LimitedInputStream(context->input, size);
	dir.read(&lis);

	ctx.apath = context->apath + '/';
	ctx.input = context->input;

	directory.create();

	for (const Directory_1::Entry &entry : dir.getChildren()) {
		ctx.apath += entry.name;

		_pullObject(&ctx);
		__applyStat(ctx.apath, entry.stat);

		ctx.apath.resize(ctx.apath.length() - entry.name.length());
	}
}

void Pull_1::_mergeDirectory(const Context *context)
{
	throw 0;
}

void Pull_1::_pullDirectory(const Context *context)
{
	struct stat st;
	int ret;

	ret = ::lstat(context->apath.c_str(), &st);
	if (ret != 0) {
		_createDirectory(context);
	} else if ((st.st_mode & S_IFMT) == S_IFDIR) {
		_mergeDirectory(context);
	} else {
		_delete(context);
		_createDirectory(context);
	}
}

void Pull_1::_pullRegular(const Context *context)
{
	LimitedInputStream lis;
	struct stat st;
	Regular_1 reg;
	uint64_t size;
	int ret;

	ret = ::lstat(context->apath.c_str(), &st);
	if ((ret == 0) && ((st.st_mode & S_IFMT) != S_IFREG))
		_delete(context);

	context->input->readall(&size, sizeof (size));
	lis = LimitedInputStream(context->input, size);

	reg = Regular_1::makeTo(context->apath);
	reg.read(&lis);
}

void Pull_1::_pullSymlink(const Context *context)
{
	struct stat st;
	Symlink_1 link;
	uint64_t size;
	int ret;

	ret = ::lstat(context->apath.c_str(), &st);
	if ((ret == 0) && ((st.st_mode & S_IFMT) != S_IFLNK))
		_delete(context);

	context->input->readall(&size, sizeof (size));

	link.read(context->input);
	link.apply(context->apath);
}

void Pull_1::pull(InputStream *input, const string &root)
{
	Context ctx;

	ctx.apath = root;
	ctx.input = input;

	_pullObject(&ctx);
}
