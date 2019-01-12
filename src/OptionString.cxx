#include "synctl/OptionString.hxx"

#include <string>

#include "synctl/Option.hxx"
#include "synctl/OptionAffectedTwiceException.hxx"
#include "synctl/OptionWithArgumentException.hxx"


using std::string;
using synctl::Option;
using synctl::OptionAffectedTwiceException;
using synctl::OptionString;
using synctl::OptionWithArgumentException;


OptionString::OptionString(const string &longName, const string &defaultValue)
	: Option(longName), _value(defaultValue)
{
}

OptionString::OptionString(char shortName, const string &defaultValue)
	: Option(shortName), _value(defaultValue)
{
}

OptionString::OptionString(const string &longName, char shortName,
			   const string &defaultValue)
	: Option(longName, shortName), _value(defaultValue)
{
}

bool OptionString::acceptArgument() const
{
	return true;
}

void OptionString::affect()
{
	throw OptionWithArgumentException();
}

void OptionString::affect(const std::string &argument)
{
	if (affected() > 0)
		throw OptionAffectedTwiceException();

	_value = argument;
	affected() += 1;
}

const string &OptionString::value() const
{
	return _value;
}
