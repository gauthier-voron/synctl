#include "synctl/server/SymlinkV1.hxx"

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
using synctl::server::SymlinkV1;


SymlinkV1::SymlinkV1()
{
}

void SymlinkV1::children(vector<Reference> *dest, InputStream *input)
{
}
		
void SymlinkV1::load(InputStream *input, OutputStream *output)
{
}

Reference SymlinkV1::store(InputStream *input, OutputStream *output,
			   vector<Reference> *children)
{
	static Type type = Type::SymlinkV1;
	HashOutputStream hos(output);
	string buffer;
	uint16_t nlen;

	input->readall(&nlen, sizeof (nlen));

	buffer.resize(nlen);
	input->readall(buffer.data(), nlen);

	hos.write(&type, sizeof (type));
	hos.write(&nlen, sizeof (nlen));
	hos.write(buffer.data(), nlen);

	return hos.digest();
}
