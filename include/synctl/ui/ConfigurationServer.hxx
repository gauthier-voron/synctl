#ifndef _INCLUDE_SYNCTL_CONFIGURATIONSERVER_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATIONSERVER_HXX_


#include <string>
#include <vector>

#include "synctl/ui/ConfigurationCommand.hxx"


namespace synctl {


class ConfigurationBase;


class ConfigurationServer : public ConfigurationCommand
{
	bool         _hasOperand;
	std::string  _operandServerPath;


 public:
	ConfigurationServer() = default;
	ConfigurationServer(ConfigurationBase *inner);
	ConfigurationServer(const ConfigurationServer &other) = default;

	ConfigurationServer &operator=(const ConfigurationServer &other) =
		default;


	bool hasServerPath() const;

	std::string serverPath() const;


	virtual size_t getOperands(const std::vector<std::string> &args);

	using ConfigurationCommand::getOptions;


	static int main(ConfigurationBase *base,
			const std::vector<std::string> &args);
};


}


#endif
