#include "synctl/client/SymlinkV1.hxx"

#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <istream>
#include <string>

#include "synctl/HashOutputStream.hxx"
#include "synctl/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/Type.hxx"


#define SYMLINK_PATH_BUFFER_SIZE   64


using std::istream;
using std::string;
using synctl::HashOutputStream;
using synctl::OutputStream;
using synctl::Reference;
using synctl::Type;
using synctl::client::SymlinkV1;


SymlinkV1::SymlinkV1(const string &path)
	: _path(path)
{
}

bool SymlinkV1::send(OutputStream *os, Reference *dest)
{
	static Type type = Type::SymlinkV1;
	HashOutputStream hos = HashOutputStream(os);
	ssize_t s, l = SYMLINK_PATH_BUFFER_SIZE;
	uint16_t nlen;
	string buf;

	buf.resize(l);

	while ((s = readlink(_path.c_str(), buf.data(), l)) == l) {
		l = l * 2;
		buf.resize(l);
	}

	if (s < 0)
		return false;

	nlen = (uint16_t) s;

	hos.write(&type, sizeof (type));
	hos.write(&nlen, sizeof (nlen));
	hos.write(buf.data(), nlen);

	hos.digest(dest);
	return true;
}

void SymlinkV1::recv(istream &is)
{
}
