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


#include "synctl/io/TransientOutputStream.hxx"

#include <unistd.h>

#include <cstdint> 
#include <cstdlib> 
#include <memory> 

#include "synctl/io/FileOutputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/repo/ObjectStore.hxx"


using std::move;
using std::string;
using std::unique_ptr;
using synctl::OutputStream;
using synctl::Reference;
using synctl::ObjectStore;
using synctl::TransientOutputStream;


TransientOutputStream::TransientOutputStream()
{
}

TransientOutputStream::TransientOutputStream(ObjectStore *ostore,
					     const string &path,
					     FileOutputStream &&output)
	: _ostore(ostore), _path(path), _output(move(output))
{
}

TransientOutputStream::TransientOutputStream(TransientOutputStream &&other)
	: _ostore(other._ostore), _path(move(other._path)),
	  _output(move(other._output))
{
	other._ostore = nullptr;
}

TransientOutputStream::~TransientOutputStream()
{
	close();
}

TransientOutputStream &
TransientOutputStream::operator=(TransientOutputStream &&other)
{
	_ostore = other._ostore;
	_path = move(other._path);
	_output = move(other._output);
	other._ostore = nullptr;

	return *this;
}

void TransientOutputStream::write(uint8_t c)
{
	_output.write(c);
}

void TransientOutputStream::write(const uint8_t *src, size_t len)
{
	_output.write(src, len);
}

void TransientOutputStream::commit(const Reference &reference)
{
	_output.close();
	_ostore->putObject(_path, reference);
	_path.clear();
}

void TransientOutputStream::flush()
{
	_output.flush();
}

void TransientOutputStream::close()
{
	if (_path.empty())
		return;

	_output.close();
	::unlink(_path.c_str());
	_path.clear();
}
