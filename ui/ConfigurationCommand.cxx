#include "synctl/ui/ConfigurationCommand.hxx"

#include <string>

#include "synctl/ui/ConfigurationBase.hxx"
#include "synctl/ui/OptionBoolean.hxx"
#include "synctl/ui/OptionString.hxx"


using std::string;
using synctl::ConfigurationCommand;
using synctl::OptionBoolean;
using synctl::OptionString;


ConfigurationCommand::ConfigurationCommand(ConfigurationBase *inner)
	: _inner(inner)
{
}

const OptionString &ConfigurationCommand::optionConfig() const
{
	return _inner->optionConfig();
}

const OptionBoolean &ConfigurationCommand::optionHelp() const
{
	return _inner->optionHelp();
}

string ConfigurationCommand::config() const
{
	return _inner->config();
}

bool ConfigurationCommand::help() const
{
	return _inner->help();
}

const string &ConfigurationCommand::command() const
{
	return _inner->command();
}

void ConfigurationCommand::getOptions(std::vector<Option *> *dest)
{
	_inner->getOptions(dest);
}

string ConfigurationCommand::seekProfile(const string &name) const
{
	return _inner->seekProfile(name);
}
