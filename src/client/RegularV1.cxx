#include "synctl/client/RegularV1.hxx"

#include <sys/stat.h>
#include <sys/types.h>

#include <cstdint>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>

#include "synctl/HashOutputStream.hxx"
#include "synctl/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/Type.hxx"
#include "synctl/client/EntryFactory.hxx"


#define TRANSFER_BUFFER_SIZE   4096


using std::ifstream;
using std::istream;
using std::ostream;
using std::string;
using synctl::HashOutputStream;
using synctl::OutputStream;
using synctl::Reference;
using synctl::Type;
using synctl::client::EntryFactory;
using synctl::client::RegularV1;


RegularV1::RegularV1(const string &path, const EntryFactory *factory)
	: _factory(factory), _path(path)
{
}

size_t RegularV1::_transfer(OutputStream *to, istream &from, size_t len)
{
	char buffer[TRANSFER_BUFFER_SIZE];
	size_t rlen, done = 0;

	while ((done < len) && from.good() && !from.eof()) {
		rlen = len - done;
		if (rlen > sizeof (buffer))
			rlen = sizeof (buffer);
		from.read(buffer, rlen);

		to->write(buffer, rlen);
		done += rlen;
	}

	return done;
}

// Format of the meta information sent for each child.
//   uint64_t     size      = S
//   S            content
bool RegularV1::send(OutputStream *os, Reference *dest)
{
	static const Type type = Type::RegularV1;
	HashOutputStream hos(os);
	ifstream ifs(_path);
	struct stat st;
	uint64_t size;
	int ret;

	if (ifs.bad())
		return false;

	ret = stat(_path.c_str(), &st);
	if (ret != 0)
		return false;

	size = st.st_size;

	hos.write(&type, sizeof (type));
	hos.write(&size, sizeof (size));

	if (_transfer(&hos, ifs, size) != size)
		return false;

	hos.digest(dest);
	return true;
}

void RegularV1::recv(istream &is)
{
}
