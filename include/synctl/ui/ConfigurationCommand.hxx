#ifndef _INCLUDE_SYNCTL_CONFIGURATIONCOMMAND_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATIONCOMMAND_HXX_


#include <string>

#include "synctl/ui/ConfigurationBase.hxx"


namespace synctl {


class OptionString;


class ConfigurationCommand : public Configurationzzz
{
	ConfigurationBase  *_inner;


 public:
	ConfigurationCommand() = default;
	ConfigurationCommand(ConfigurationBase *inner);
	ConfigurationCommand(const ConfigurationCommand &other) = default;

	ConfigurationCommand &operator=(const ConfigurationCommand &other) =
		default;


	const OptionString &getOptionConfig() const;

	const std::string &getConfig() const;


	const std::string &getCommand() const;


	virtual void getOptions(std::vector<Option *> *dest);

	using Configurationzzz::getOptions;
};


}


#endif
