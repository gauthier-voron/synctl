#ifndef _INCLUDE_SYNCTL_CONFIGURATIONCOMMAND_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATIONCOMMAND_HXX_


#include <string>

#include "synctl/ui/ConfigurationBase.hxx"


namespace synctl {


class OptionString;


class ConfigurationCommand : public Configuration
{
	ConfigurationBase  *_inner;


 public:
	ConfigurationCommand() = default;
	ConfigurationCommand(ConfigurationBase *inner);
	ConfigurationCommand(const ConfigurationCommand &other) = default;

	ConfigurationCommand &operator=(const ConfigurationCommand &other) =
		default;


	const OptionString &optionConfig() const;

	std::string config() const;


	const std::string &command() const;


	virtual void getOptions(std::vector<Option *> *dest);

	using Configuration::getOptions;


	std::string seekProfile(const std::string &name) const;
};


}


#endif
