#ifndef _INCLUDE_SYNCTL_CONFIGURATIONINIT_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATIONINIT_HXX_


#include <string>
#include <vector>

#include "synctl/ui/ConfigurationCommand.hxx"
#include "synctl/ui/OptionBoolean.hxx"


namespace synctl {


class ConfigurationBase;


class ConfigurationInit : public ConfigurationCommand
{
	OptionBoolean  _optionForce = OptionBoolean("force", 'f');
	std::string    _operandServerPath;


 public:
	ConfigurationInit() = default;
	ConfigurationInit(ConfigurationBase *inner);
	ConfigurationInit(const ConfigurationInit &other) = default;

	ConfigurationInit &operator=(const ConfigurationInit &other) = default;


	const OptionBoolean &optionForce() const;

	bool force() const;

	const std::string &serverPath() const;


	virtual size_t getOperands(const std::vector<std::string> &args);

	virtual void getOptions(std::vector<Option *> *dest);

	using ConfigurationCommand::getOptions;


	static int main(ConfigurationBase *base,
			const std::vector<std::string> &args);
};


}


#endif
