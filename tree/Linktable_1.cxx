// Copyright 2019 Gauthier Voron
//
// This file is part of Synctl
//
// Synctl is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Fundation, either version 3 of the License, or (at your option) any later
// version.
//
// Synctl is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANDABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// Synctl. If not, see <https://www.gnu.org/licenses/>.
//


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
