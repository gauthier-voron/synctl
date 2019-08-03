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


#include "synctl/tree/Symlink_1.hxx"

#include <unistd.h>

#include <string>

#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/IOException.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/io/Symlink.hxx"
#include "synctl/tree/Reference.hxx"


using std::string;
using synctl::HashOutputStream;
using synctl::InputStream;
using synctl::IOException;
using synctl::OutputStream;
using synctl::Symlink;
using synctl::Symlink_1;
using synctl::Reference;


Symlink_1::Symlink_1(const string &target)
	: _target(target)
{
}

void Symlink_1::_write(OutputStream *output) const
{
	output->writeStr(_target);
}

void Symlink_1::_read(InputStream *input)
{
	input->readStr(&_target);
}

void Symlink_1::setTarget(const string &target)
{
	_target = target;
}

string Symlink_1::getTarget() const
{
	return _target;
}

void Symlink_1::write(OutputStream *output, Reference *ref) const
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

void Symlink_1::read(InputStream *input, Reference *ref)
{
	if (ref != nullptr)
		throw 0;  // not yet implemented
	_read(input);
}

Symlink_1 Symlink_1::make(const string &path)
{
	Symlink symlink = Symlink(path);
	return Symlink_1(symlink.get());
}


void Symlink_1::apply(const string &path) const
{
	Symlink symlink = Symlink(path);

	if (symlink.exists())
		symlink.set(_target);
	else
		symlink.create(_target);
}
