#include "synctl/Action.hxx"

#include <string>
#include <vector>

#include "synctl/Option.hxx"


using std::string;
using std::vector;
using synctl::Action;
using synctl::Option;


void Action::addOption(Option *option)
{
	_options.push_back(option);
}

Action::Action(const string &name)
	: _name(name)
{
}

const string &Action::name() const noexcept
{
	return _name;
}

const vector<Option *> &Action::options() const noexcept
{
	return _options;
}
