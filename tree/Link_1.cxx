#include "synctl/tree/Link_1.hxx"

#include <set>
#include <string>

#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Reference.hxx"


using std::set;
using std::string;
using synctl::InputStream;
using synctl::Link_1;
using synctl::OutputStream;


void Link_1::_write(OutputStream *output) const
{
	for (const string &location : _locations)
		output->writeStr(location);
	output->writeStr("");
}

void Link_1::_read(InputStream *input)
{
	string location;

	while ((location = input->readStr()) != "")
		_locations.insert(location);
}

void Link_1::addLocation(const string &name)
{
	if (name.empty())
		throw 0;
	_locations.insert(name);
}

void Link_1::removeLocation(const string &name)
{
	auto it = _locations.find(name);

	if (it != _locations.end())
		_locations.erase(it);
}

const set<string> &Link_1::getLocations() const
{
	return _locations;
}

void Link_1::clear()
{
	_locations.clear();
}

void Link_1::write(OutputStream *output, Reference *ref) const
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

void Link_1::read(InputStream *input, Reference *ref)
{
	if (ref != nullptr)
		throw 0;  // not yet implemented
	_read(input);
}
