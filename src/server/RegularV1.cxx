#include "synctl/server/RegularV1.hxx"

#include <cstdint>
#include <vector>

#include "synctl/HashOutputStream.hxx"
#include "synctl/InputStream.hxx"
#include "synctl/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/Type.hxx"


#define TRANSFER_BUFFER_SIZE   4096


using std::vector;
using synctl::HashOutputStream;
using synctl::InputStream;
using synctl::OutputStream;
using synctl::Reference;
using synctl::Type;
using synctl::server::RegularV1;


void RegularV1::_transfer(InputStream *in, OutputStream *out, size_t len)
{
	char buffer[TRANSFER_BUFFER_SIZE];
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

RegularV1::RegularV1()
{
}

void RegularV1::children(vector<Reference> *dest, InputStream *input)
{
}

void RegularV1::load(InputStream *input, OutputStream *output)
{
}

Reference RegularV1::store(InputStream *input, OutputStream *output,
			   vector<Reference> *children)
{
	static Type type = Type::RegularV1;
	HashOutputStream hos(output);
	uint64_t size;

	input->readall(&size, sizeof (size));

	hos.write(&type, sizeof (type));
	hos.write(&size, sizeof(size));
	_transfer(input, &hos, size);

	return hos.digest();
}
