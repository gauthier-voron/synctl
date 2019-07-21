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


#include "synctl/tree/FilterCodec.hxx"

#include <memory>

#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/tree/FirstMatchFilter.hxx"


using std::make_unique;
using std::unique_ptr;
using synctl::Filter;
using synctl::FilterCodec;
using synctl::FirstMatchFilter;
using synctl::InputStream;
using synctl::OutputStream;


void FilterCodec::encode(const Filter *filter, OutputStream *output) const
{
	opcode_t op;

	if (filter == nullptr) {
		op = OP_FLT_NONE;
	} else if (dynamic_cast<const FirstMatchFilter *> (filter)) {
		op = OP_FLT_FMATCH;
	} else {
		throw 0;
	}

	output->writeInt(op);

	if (filter != nullptr)
		filter->write(output);
}

unique_ptr<Filter> FilterCodec::decode(InputStream *input) const
{
	unique_ptr<Filter> ret;
	opcode_t op;

	op = input->readInt<opcode_t>();

	switch (op) {
	case OP_FLT_NONE:   ret = nullptr; break;
	case OP_FLT_FMATCH: ret = make_unique<FirstMatchFilter>(); break;
	default:            throw 0;
	}

	if (ret != nullptr)
		ret->read(input);

	return ret;
}
