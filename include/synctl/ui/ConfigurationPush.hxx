#ifndef _INCLUDE_SYNCTL_CONFIGURATIONPUSH_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATIONPUSH_HXX_


#include <string>
#include <vector>

#include "synctl/tree/FirstMatchFilter.hxx"
#include "synctl/ui/ConfigurationCommand.hxx"
#include "synctl/ui/OptionBoolean.hxx"
#include "synctl/ui/OptionLambda.hxx"
#include "synctl/ui/OptionString.hxx"


namespace synctl {


class ConfigurationBase;
class Filter;


class ConfigurationPush : public ConfigurationCommand
{
	OptionString   _optionBranch = OptionString("branch", 'b');
	OptionString   _optionCommand = OptionString("command", 'c');
	OptionLambda   _optionExclude;
	OptionLambda   _optionInclude;
	OptionString   _optionRoot = OptionString("root", 'r');
	OptionString   _optionServer = OptionString("server", 's');
	OptionString   _optionTrunk = OptionString("trunk", 't');
	OptionBoolean  _optionVerbose = OptionBoolean("verbose", 'v');

	bool           _hasProfile = false;
	std::string    _operandProfile;

	FirstMatchFilter  _filter;


 public:
	ConfigurationPush() = default;
	ConfigurationPush(ConfigurationBase *inner);
	ConfigurationPush(const ConfigurationPush &other) = default;

	ConfigurationPush &operator=(const ConfigurationPush &other) = default;


	const OptionString &optionBranch() const;
	const OptionString &optionCommand() const;
	const OptionString &optionRoot() const;
	const OptionString &optionServer() const;
	const OptionString &optionTrunk() const;
	const OptionBoolean &optionVerbose() const;
	bool hasProfile() const;

	const std::string &branch() const;
	const std::string &command() const;
	Filter *filter();
	const std::string &root() const;
	const std::string &server() const;
	const std::string &trunk() const;
	bool verbose() const;
	std::string profile() const;


	virtual size_t getOperands(const std::vector<std::string> &args);

	virtual void getOptions(std::vector<Option *> *dest);

	using ConfigurationCommand::getOptions;


	static int main(ConfigurationBase *base,
			const std::vector<std::string> &args);
};


}


#endif
