#include "synctl/FirstMatchFilter.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/Filter.hxx"
#include "synctl/Pattern.hxx"


using std::make_pair;
using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::Filter;
using synctl::FirstMatchFilter;
using synctl::Pattern;


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
