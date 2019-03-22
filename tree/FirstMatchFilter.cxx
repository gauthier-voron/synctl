#include "synctl/tree/FirstMatchFilter.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/tree/Pattern.hxx"
#include "synctl/tree/PatternCodec.hxx"


using std::make_pair;
using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::Filter;
using synctl::FirstMatchFilter;
using synctl::InputStream;
using synctl::OutputStream;
using synctl::Pattern;
using synctl::PatternCodec;


FirstMatchFilter::FirstMatchFilter()
{
}

FirstMatchFilter::~FirstMatchFilter()
{
}

void FirstMatchFilter::append(unique_ptr<Pattern> pattern, Filter::Action act)
{
	_rules.emplace_back(make_pair(move(pattern), move(act)));
}

Filter::Action FirstMatchFilter::apply(const string &path)
{
	for (auto &rule : _rules)
		if (rule.first->match(path))
			return rule.second;
	return Filter::Action::Ignore;
}

void FirstMatchFilter::write(OutputStream *output) const
{
	PatternCodec codec;
	uint8_t actcode;

	for (auto &rule : _rules) {
		switch (rule.second) {
		case Ignore: actcode = '='; break;
		case Accept: actcode = '+'; break;
		case Reject: actcode = '-'; break;
		case Traverse: actcode = '>'; break;
		}

		output->writeInt(actcode);
		codec.encode(rule.first.get(), output);
	}

	actcode = 0;
	output->writeInt(actcode);
}

void FirstMatchFilter::read(InputStream *input)
{
	unique_ptr<Pattern> pattern;
	PatternCodec codec;
	Filter::Action act;
	uint16_t actcode;

	_rules.clear();

	while ((actcode = input->readInt<uint8_t>()) > 0) {
		switch ((uint8_t) actcode) {
		case '=': act = Filter::Ignore; break;
		case '+': act = Filter::Accept; break;
		case '-': act = Filter::Reject; break;
		case '>': act = Filter::Traverse; break;
		default: throw 0;
		}

		pattern = codec.decode(input);
		append(move(pattern), act);
	}

	if (actcode != 0)
		throw 0;
}
