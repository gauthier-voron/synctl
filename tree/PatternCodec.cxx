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


#include "synctl/tree/PatternCodec.hxx"

#include <memory>

#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/GlobPattern.hxx"
#include "synctl/tree/Pattern.hxx"


using std::make_unique;
using std::unique_ptr;
using synctl::GlobPattern;
using synctl::InputStream;
using synctl::OutputStream;
using synctl::Pattern;
using synctl::PatternCodec;


void PatternCodec::encode(const Pattern *pattern, OutputStream *output) const
{
	opcode_t op;

	if (pattern == nullptr) {
		op = OP_PAT_NONE;
	} else if (dynamic_cast<const GlobPattern *> (pattern)) {
		op = OP_PAT_GLOB;
	} else {
		throw 0;
	}

	output->writeInt(op);

	if (pattern != nullptr)
		pattern->write(output);
}

unique_ptr<Pattern> PatternCodec::decode(InputStream *input) const
{
	unique_ptr<Pattern> ret;
	opcode_t op;

	op = input->readInt<opcode_t>();

	switch (op) {
	case OP_PAT_NONE: ret = nullptr; break;
	case OP_PAT_GLOB: ret = make_unique<GlobPattern>(); break;
	default:          throw 0;
	}

	if (ret != nullptr)
		ret->read(input);

	return ret;
}
