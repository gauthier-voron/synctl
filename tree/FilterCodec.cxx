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

	output->write(&op, sizeof (op));

	if (filter != nullptr)
		filter->write(output);
}

unique_ptr<Filter> FilterCodec::decode(InputStream *input) const
{
	unique_ptr<Filter> ret;
	opcode_t op;

	input->readall(&op, sizeof (op));

	switch (op) {
	case OP_FLT_NONE:   ret = nullptr; break;
	case OP_FLT_FMATCH: ret = make_unique<FirstMatchFilter>(); break;
	default:            throw 0;
	}

	if (ret != nullptr)
		ret->read(input);

	return ret;
}
