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

string ConfigurationCommand::getConfig() const
{
	return _inner->config();
}

const string &ConfigurationCommand::getCommand() const
{
	return _inner->getCommand();
}

void ConfigurationCommand::getOptions(std::vector<Option *> *dest)
{
	_inner->getOptions(dest);
}

string ConfigurationCommand::seekProfile(const string &name) const
{
	return _inner->seekProfile(name);
}
