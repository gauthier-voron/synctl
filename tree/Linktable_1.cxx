#include "synctl/tree/Linktable_1.hxx"

#include <set>

#include "synctl/io/EOFException.hxx"
#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Reference.hxx"


using std::set;
using synctl::EOFException;
using synctl::HashOutputStream;
using synctl::InputStream;
using synctl::Linktable_1;
using synctl::OutputStream;
using synctl::Reference;


void Linktable_1::_write(OutputStream *output) const
{
	for (const Reference &reference : _links)
		output->write(reference.data(), reference.size());
}

void Linktable_1::_read(InputStream *input)
{
	Reference reference;

	while (true) {
		try {
			input->readall(reference.data(), reference.size());
			_links.insert(reference);
		} catch (EOFException &) {
			break;
		}
	}
}

void Linktable_1::addLink(const Reference &reference)
{
	_links.insert(reference);
}

void Linktable_1::removeLink(const Reference &reference)
{
	auto it = _links.find(reference);

	if (it != _links.end())
		_links.erase(it);
}

const set<Reference> &Linktable_1::getLinks() const
{
	return _links;
}

size_t Linktable_1::size() const
{
	return _links.size();
}

void Linktable_1::write(OutputStream *output, Reference *ref) const
{
	HashOutputStream hos;

	if (ref == nullptr) {
		_write(output);
	} else {
		hos = HashOutputStream(output);
		_write(&hos);
		hos.digest(ref);
	}
}

void Linktable_1::read(InputStream *input, Reference *ref)
{
	if (ref != nullptr)
		throw 0;  // not yet implemented
	_read(input);
}
