#include "synctl/ui/OptionLambda.hxx"

#include <functional>
#include <string>

#include "synctl/ui/OptionWithArgumentException.hxx"


using std::function;
using std::string;
using synctl::OptionLambda;
using synctl::OptionWithArgumentException;


OptionLambda::OptionLambda(const string &longName,
			   function<void(const string &)> lambda)
	: Option(longName), _lambda(lambda)
{
}

OptionLambda::OptionLambda(char shortName,
			   function<void(const string &)> lambda)
	: Option(shortName), _lambda(lambda)
{
}

OptionLambda::OptionLambda(const string &longName, char shortName,
			   function<void(const string &)> lambda)
	: Option(longName, shortName), _lambda(lambda)
{
}

bool OptionLambda::acceptArgument() const
{
	return true;
}

void OptionLambda::affect()
{
	throw OptionWithArgumentException();
}

void OptionLambda::affect(const string &argument)
{
	_lambda(argument);
}
