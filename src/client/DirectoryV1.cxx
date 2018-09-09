#include "synctl/client/DirectoryV1.hxx"

#include <dirent.h>
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
#include "synctl/HashOutputStream.hxx"
#include "synctl/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/Type.hxx"
#include "synctl/client/EntryFactory.hxx"
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
using synctl::HashOutputStream;
using synctl::OutputStream;
using synctl::Reference;
using synctl::Type;
using synctl::client::DirectoryV1;
using synctl::client::EntryFactory;
using synctl::client::Sender;


DirectoryV1::DirectoryV1(const string &path, const EntryFactory *factory)
	: _factory(factory), _path(path)
{
}


bool DirectoryV1::_send_child(OutputStream *os, const string &p, Reference *r)
	const
{
	unique_ptr<Sender> sender = _factory->instance(p);

	if (sender == nullptr)
		return false;

	return sender->send(os, r);
}

// Format of the meta information sent for each child.
//   uint8_t      st_dev
//   uint8_t      st_ino
//   uint16_t     st_mode
//   uint16_t     username.length()    = A
//   uint16_t     groupname.length()   = B
//   uint16_t     childname.length()   = C
//   uint8_t[20]  reference
//   C            username
//   C            groupname
//   C            childname
void DirectoryV1::_send_child_meta(OutputStream *os, const string &name,
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

	os->write(&meta, sizeof (meta));
	os->write(ref.data(), ref.size());
	os->write(username.data(), username.size());
	os->write(groupname.data(), groupname.size());
	os->write(name.data(), name.size());
}

#include <iostream>
bool DirectoryV1::send(OutputStream *os, Reference *dest)
{
	static const Type type = Type::DirectoryV1;
	HashOutputStream hos(os);
	AdapterOutputStream aos;
	string epath, cpath;
	ostringstream buffer;
	Reference placeholder;
	struct dirent *de;
	struct stat st;
	uint64_t n;
	DIR *dd;
	int ret;

	dd = opendir(_path.c_str());
	if (dd == NULL)
		return false;

	aos.assign(buffer);

	n = 0;

	while ((de = readdir(dd)) != NULL) {
		cpath.assign(de->d_name);
		if ((cpath == ".") || (cpath == ".."))
			continue;
		
		epath = _path + '/' + cpath;

		ret = lstat(epath.c_str(), &st);
		if (ret != 0)
			continue;

		if (!_send_child(os, epath, &placeholder))
			continue;

		n += 1;
		_send_child_meta(&aos, cpath, placeholder, st);
	}

	closedir(dd);

	hos.write(&type, sizeof (type));
	hos.write(&n, sizeof (n));
	hos.write(buffer.str().data(), buffer.str().length());
	hos.digest(dest);

	std::cerr << dest->toHex() << std::endl;
	return true;
}

void DirectoryV1::recv(istream &is)
{
}
