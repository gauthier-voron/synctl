#include "synctl/server/DirectoryV1.hxx"

#include <cstdint>
#include <string>
#include <vector>

#include "synctl/HashOutputStream.hxx"
#include "synctl/InputStream.hxx"
#include "synctl/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/Type.hxx"


using std::string;
using std::vector;
using synctl::HashOutputStream;
using synctl::InputStream;
using synctl::OutputStream;
using synctl::Reference;
using synctl::Type;
using synctl::server::DirectoryV1;


#include <iostream>


void DirectoryV1::_transfer(InputStream *in, OutputStream *out, Reference *ref)
{
	string user, group, name;

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

	in->readall(&meta, sizeof (meta));
	in->readall(ref->data(), ref->size());

	user.resize(meta.ulen);
	group.resize(meta.glen);
	name.resize(meta.nlen);

	in->readall(user.data(), meta.ulen);
	in->readall(group.data(), meta.glen);
	in->readall(name.data(), meta.nlen);

	out->write(&meta, sizeof (meta));
	out->write(ref->data(), ref->size());
	out->write(user.data(), meta.ulen);
	out->write(group.data(), meta.glen);
	out->write(name.data(), meta.nlen);
}

DirectoryV1::DirectoryV1()
{
}

void DirectoryV1::children(vector<Reference> *dest, InputStream *input)
{
}
		
void DirectoryV1::load(InputStream *input, OutputStream *output)
{
}

Reference DirectoryV1::store(InputStream *input, OutputStream *output,
			     vector<Reference> *children)
{
	static Type type = Type::DirectoryV1;
	HashOutputStream hos(output);
	Reference *childref;
	uint64_t i, size;

	input->readall(&size, sizeof (size));

	hos.write(&type, sizeof (type));
	hos.write(&size, sizeof (size));

	for (i = 0; i < size; i++) {
		childref = &children->emplace_back();
		_transfer(input, &hos, childref);
	}

	return hos.digest();
}
