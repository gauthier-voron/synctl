#ifndef _INCLUDE_SYNCTL_CONFIGURATIONHELP_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATIONHELP_HXX_


#include <string>
#include <vector>

#include "synctl/ui/ConfigurationCommand.hxx"
#include "synctl/ui/OptionBoolean.hxx"


namespace synctl {


class ConfigurationBase;


class ConfigurationHelp : public ConfigurationCommand
{
	OptionBoolean  _optionProfile  = OptionBoolean("profile");
	OptionBoolean  _optionRegex    = OptionBoolean("regex");
	OptionBoolean  _optionTutorial = OptionBoolean("tutorial");
	bool           _hasCommand;
	std::string    _operandCommand;


 public:
	ConfigurationHelp() = default;
	ConfigurationHelp(ConfigurationBase *inner);
	ConfigurationHelp(const ConfigurationHelp &other) = default;

	ConfigurationHelp &operator=(const ConfigurationHelp &other) = default;


	const OptionBoolean &optionProfile() const;
	const OptionBoolean &optionRegex() const;
	const OptionBoolean &optionTutorial() const;
	bool hasCommand() const;
	size_t hasOptions() const;

	bool profile() const;
	bool regex() const;
	bool tutorial() const;
	const std::string &command() const;


	virtual size_t getOperands(const std::vector<std::string> &args);

	virtual void getOptions(std::vector<Option *> *dest);

	using ConfigurationCommand::getOptions;


	static void displayProfileHelp();
	static void displayRegexHelp();
	static void displayTutorialHelp();
	static void displayMainHelp();
	static void displayCommandHelp(const std::string &command);

	static int main(ConfigurationBase *base,
			const std::vector<std::string> &args);
};


}


#endif
