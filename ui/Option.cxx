#include "synctl/ui/Option.hxx"


using std::string;
using synctl::Option;


Option::Option(const std::string &longName)
	: _longName(longName)
{
}

Option::Option(char shortName)
	: _shortName(shortName)
{
}

Option::Option(const std::string &longName, char shortName)
	: _longName(longName), _shortName(shortName)
{
}

const string &Option::longName() const
{
	return _longName;
}

char Option::shortName() const
{
	return _shortName;
}

void Option::affect()
{
	_affected += 1;
}

size_t &Option::affected()
{
	return _affected;
}

const size_t &Option::affected() const
{
	return _affected;
}
