#include "synctl/ui/OptionListString.hxx"

#include <string>
#include <vector>

#include "synctl/ui/Option.hxx"
#include "synctl/ui/OptionWithArgumentException.hxx"


using std::string;
using std::vector;
using synctl::Option;
using synctl::OptionListString;
using synctl::OptionWithArgumentException;


OptionListString::OptionListString(const string &longName)
	: Option(longName)
{
}

OptionListString::OptionListString(char shortName)
	: Option(shortName)
{
}

OptionListString::OptionListString(const string &longName, char shortName)
	: Option(longName, shortName)
{
}

bool OptionListString::acceptArgument() const
{
	return true;
}

void OptionListString::affect()
{
	throw OptionWithArgumentException();
}

void OptionListString::affect(const std::string &argument)
{
	_values.push_back(argument);
	affected() += 1;
}

const vector<string> &OptionListString::values() const
{
	return _values;
}
