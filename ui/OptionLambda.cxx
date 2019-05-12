#include "synctl/ui/OptionLambda.hxx"

#include <functional>
#include <string>

#include "synctl/ui/OptionWithArgumentException.hxx"
#include "synctl/ui/OptionWithoutArgumentException.hxx"


using std::function;
using std::string;
using synctl::OptionLambda;
using synctl::OptionWithArgumentException;
using synctl::OptionWithoutArgumentException;


OptionLambda::OptionLambda(const string &longName,
			   function<void()> lambda)
	: Option(longName), _withArg(false), _lambdaWithoutArg(lambda)
{
}

OptionLambda::OptionLambda(char shortName,
			   function<void()> lambda)
	: Option(shortName), _withArg(false), _lambdaWithoutArg(lambda)
{
}

OptionLambda::OptionLambda(const string &longName, char shortName,
			   function<void()> lambda)
	: Option(longName, shortName), _withArg(false)
	, _lambdaWithoutArg(lambda)
{
}

OptionLambda::OptionLambda(const string &longName,
			   function<void(const string &)> lambda)
	: Option(longName), _withArg(true), _lambdaWithArg(lambda)
{
}

OptionLambda::OptionLambda(char shortName,
			   function<void(const string &)> lambda)
	: Option(shortName), _withArg(true), _lambdaWithArg(lambda)
{
}

OptionLambda::OptionLambda(const string &longName, char shortName,
			   function<void(const string &)> lambda)
	: Option(longName, shortName), _withArg(true), _lambdaWithArg(lambda)
{
}

bool OptionLambda::acceptArgument() const
{
	return _withArg;
}

void OptionLambda::affect()
{
	if (_withArg)
		throw OptionWithArgumentException();
	_lambdaWithoutArg();
}

void OptionLambda::affect(const string &argument)
{
	if (_withArg == false)
		throw OptionWithoutArgumentException();
	_lambdaWithArg(argument);
}
