#ifndef _INCLUDE_SYNCTL_CONFIGURATION_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATION_HXX_


#include <vector>

#include "synctl/ui/Option.hxx"


namespace synctl {


class Configuration
{
 public:
	virtual ~Configuration() = default;


	virtual size_t getOperands(const std::vector<std::string> &args) = 0;

	virtual void getOptions(std::vector<Option *> *dest) = 0;

	virtual std::vector<Option *> getOptions();
};


}


#endif
