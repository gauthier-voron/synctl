#include "synctl/client/DirectoryV1.hxx"

#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <filesystem>
#include <istream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "synctl/AdapterOutputStream.hxx"
#include "synctl/Directory.hxx"
#include "synctl/Filter.hxx"
#include "synctl/HashOutputStream.hxx"
#include "synctl/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/Type.hxx"
#include "synctl/client/SendContext.hxx"
#include "synctl/client/Sender.hxx"


using std::istream;
using std::ostream;
using std::ostringstream;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::unique_ptr;
using std::vector;
using synctl::AdapterOutputStream;
using synctl::Directory;
using synctl::Filter;
using synctl::HashOutputStream;
using synctl::OutputStream;
using synctl::Reference;
using synctl::Type;
using synctl::client::DirectoryV1;
using synctl::client::SendContext;
using synctl::client::Sender;


bool DirectoryV1::_send_child(OutputStream *out, const string &name,
			      Reference *dest, SendContext *context) const
{
	return context->send(out, dest, name);
}

void DirectoryV1::_send_child_meta(OutputStream *bout, const string &name, 
				   const Reference &ref, const struct stat &st)
	const
{
	string username, groupname;
	struct passwd *passwd;
	struct group *group;

        #pragma pack(1)
	struct
	{
		uint8_t   dev;
		uint8_t   ino;
		uint16_t  mode;
		uint16_t  ulen;
		uint16_t  glen;
		uint16_t  nlen;
	} meta;

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
	meta.ulen = username.size();
	meta.glen = groupname.size();
	meta.nlen = name.size();

	bout->write(&meta, sizeof (meta));
	bout->write(ref.data(), ref.size());
	bout->write(username.data(), username.size());
	bout->write(groupname.data(), groupname.size());
	bout->write(name.data(), name.size());
}

size_t DirectoryV1::_send_children(OutputStream *out, OutputStream *bout,
				   SendContext *context) const
{
	Directory dir = Directory(context->path());
	size_t size = 0;
	struct stat st;
	Reference ref;
	string path;
	int ret;

	if (dir.listable() == false)
		return 0;

	for (const string &name : dir.sortedTrueChildren()) {
		path = context->path() + "/" + name;

		ret = lstat(path.c_str(), &st);
		if (ret != 0)
			continue;

		if (!_send_child(out, name, &ref, context))
			continue;

		size += 1;
		_send_child_meta(bout, name, ref, st);
	}

	return size;
}

bool DirectoryV1::_send(OutputStream *out, Reference *dest, SendContext *ctx,
			bool traverse) const
{
	static const Type type = Type::DirectoryV1;
	HashOutputStream hos(out);
	AdapterOutputStream aos;
	ostringstream buffer;
	uint64_t size;

	aos.assign(buffer);
	size = _send_children(out, &aos, ctx);

	if (traverse && (size == 0))
		return false;

	hos.write(&type, sizeof (type));
	hos.write(&size, sizeof (size));
	hos.write(buffer.str().data(), buffer.str().length());
	hos.digest(dest);

	return true;
}

bool DirectoryV1::send(OutputStream *out, Reference *dest, SendContext *ctx)
{
	return _send(out, dest, ctx, false);
}

bool DirectoryV1::traverse(OutputStream *out, Reference *dst, SendContext *ctx)
{
	return _send(out, dst, ctx, true);
}

void DirectoryV1::recv(istream &is)
{
}
