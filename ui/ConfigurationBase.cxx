#include "synctl/ui/ConfigurationBase.hxx"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "synctl/default.hxx"
#include "synctl/ui/OperandMissingException.hxx"
#include "synctl/ui/OptionLambda.hxx"
#include "synctl/ui/OptionString.hxx"
#include "synctl/ui/ProfileSeeker.hxx"


using std::cout;
using std::endl;
using std::string;
using std::vector;
using synctl::ConfigurationBase;
using synctl::OptionLambda;
using synctl::OptionString;
using synctl::ProfileSeeker;


void ConfigurationBase::_displayHelp() const
{
	cout << "rtfm" << endl;
}

void ConfigurationBase::_displayVersion() const
{
	cout << SYNCTL_CONSTANT_NAME;
	cout << " ";

	cout << SYNCTL_CONSTANT_MAJOR_VERSION;
	cout << ".";
	cout << SYNCTL_CONSTANT_MINOR_VERSION;
	cout << ".";
	cout << SYNCTL_CONSTANT_VARIANT;
	cout << endl;

	cout << SYNCTL_CONSTANT_AUTHOR;
	cout << endl;

	cout << SYNCTL_CONSTANT_MAILTO;
	cout << endl;
}

ConfigurationBase::ConfigurationBase()
	: _optionHelp("help", 'h', [&]() {
		_displayHelp();
		::exit(0);
	})
	, _optionVersion("version", 'V', [&]() {
		_displayVersion();
		::exit(0);
	})
{
}

const OptionString &ConfigurationBase::getOptionConfig() const
{
	return _optionConfig;
}

string ConfigurationBase::config() const
{
	char *env;

	if (_optionConfig.affected())
		return _optionConfig.value();

	if ((env = getenv("SYNCTL_PROFILE_PATH")) != NULL)
		return env;

	return SYNCTL_DEFAULT_CONFIG;
}

const string &ConfigurationBase::getCommand() const
{
	return _operandCommand;
}

size_t ConfigurationBase::getOperands(const vector<string> &arguments)
{
	if (arguments.size() < 1)
		throw OperandMissingException("command");

	_operandCommand = arguments[0];

	return 1;
}

void ConfigurationBase::getOptions(vector<Option *> *dest)
{
	dest->push_back(&_optionConfig);
	dest->push_back(&_optionHelp);
	dest->push_back(&_optionVersion);
}

string ConfigurationBase::seekProfile(const string &name) const
{
	ProfileSeeker seeker;

	seeker.push(config());

	return seeker.seek(name);
}
