#ifndef _INCLUDE_SYNCTL_CONFIGURATIONBASE_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATIONBASE_HXX_


#include <string>

#include "synctl/ui/Configuration.hxx"
#include "synctl/ui/OptionLambda.hxx"
#include "synctl/ui/OptionString.hxx"


namespace synctl {


class ConfigurationBase : public Configuration
{
	OptionString  _optionConfig = OptionString('C');
	OptionLambda  _optionHelp;
	OptionLambda  _optionVersion;
	std::string   _operandCommand;


	void _displayHelp() const;

	void _displayVersion() const;


 public:
	ConfigurationBase();


	const OptionString &getOptionConfig() const;

	const std::string &getConfig() const;


	const std::string &getCommand() const;


	virtual size_t getOperands(const std::vector<std::string> &arguments);

	virtual void getOptions(std::vector<Option *> *dest);

	using Configuration::getOptions;

};


}


#endif
