#ifndef _INCLUDE_SYNCTL_CONFIGURATIONPULL_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATIONPULL_HXX_


#include <string>
#include <vector>

#include "synctl/tree/FirstMatchFilter.hxx"
#include "synctl/ui/ConfigurationCommand.hxx"
#include "synctl/ui/OptionLambda.hxx"
#include "synctl/ui/OptionString.hxx"


namespace synctl {


class ConfigurationBase;
class Filter;


class ConfigurationPull : public ConfigurationCommand
{
	OptionString  _optionCommand = OptionString("command", 'c');
	OptionLambda  _optionExclude;
	OptionLambda  _optionInclude;
	OptionString  _optionRoot = OptionString("root", 'r');
	OptionString  _optionServer = OptionString("server", 's');
	OptionString  _optionTrunk = OptionString("trunk", 't');

	bool          _hasProfile = false;
	std::string   _operandProfile;

	FirstMatchFilter  _filter;


 public:
	ConfigurationPull() = default;
	ConfigurationPull(ConfigurationBase *inner);
	ConfigurationPull(const ConfigurationPull &other) = default;

	ConfigurationPull &operator=(const ConfigurationPull &other) = default;


	const OptionString &optionCommand() const;
	const OptionString &optionRoot() const;
	const OptionString &optionServer() const;
	const OptionString &optionTrunk() const;
	bool hasProfile() const;

	const std::string &command() const;
	Filter *filter();
	const std::string &root() const;
	const std::string &server() const;
	const std::string &trunk() const;
	std::string profile() const;


	virtual size_t getOperands(const std::vector<std::string> &args);

	virtual void getOptions(std::vector<Option *> *dest);

	using ConfigurationCommand::getOptions;


	static int main(ConfigurationBase *base,
			const std::vector<std::string> &args);
};


}


#endif
