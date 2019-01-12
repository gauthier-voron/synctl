#include "synctl/OptionBoolean.hxx"

#include <string>

#include "synctl/Option.hxx"
#include "synctl/OptionAffectedTwiceException.hxx"
#include "synctl/OptionWithoutArgumentException.hxx"


using std::string;
using synctl::Option;
using synctl::OptionAffectedTwiceException;
using synctl::OptionBoolean;
using synctl::OptionWithoutArgumentException;


OptionBoolean::OptionBoolean(const std::string &longName)
	: Option(longName)
{
}

OptionBoolean::OptionBoolean(char shortName)
	: Option(shortName)
{
}

OptionBoolean::OptionBoolean(const std::string &longName, char shortName)
	: Option(longName, shortName)
{
}

bool OptionBoolean::acceptArgument() const
{
	return false;
}

void OptionBoolean::affect()
{
	if (affected() > 0)
		throw OptionAffectedTwiceException();

	_value = true;
	affected() += 1;
}

void OptionBoolean::affect(const std::string &argument [[gnu::unused]])
{
	throw OptionWithoutArgumentException();
}

bool OptionBoolean::value() const
{
	return _value;
}
