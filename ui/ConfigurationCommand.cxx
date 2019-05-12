#include "synctl/ui/ConfigurationCommand.hxx"

#include <string>

#include "synctl/ui/ConfigurationBase.hxx"
#include "synctl/ui/OptionString.hxx"


using std::string;
using synctl::ConfigurationCommand;
using synctl::OptionString;


ConfigurationCommand::ConfigurationCommand(ConfigurationBase *inner)
	: _inner(inner)
{
}

const OptionString &ConfigurationCommand::getOptionConfig() const
{
	return _inner->getOptionConfig();
}

const string &ConfigurationCommand::getConfig() const
{
	return _inner->getConfig();
}

const string &ConfigurationCommand::getCommand() const
{
	return _inner->getCommand();
}

void ConfigurationCommand::getOptions(std::vector<Option *> *dest)
{
	_inner->getOptions(dest);
}
