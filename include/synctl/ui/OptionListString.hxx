#ifndef _INCLUDE_SYNCTL_OPTIONLISTSTRING_HXX_
#define _INCLUDE_SYNCTL_OPTIONLISTSTRING_HXX_


#include <string>
#include <vector>

#include "synctl/ui/Option.hxx"


namespace synctl {


class OptionListString : public Option
{
	std::vector<std::string>  _values;


 public:
	OptionListString(const std::string &longName);
	OptionListString(char shortName);
	OptionListString(const std::string &longName, char shortName);


	virtual bool acceptArgument() const override;

	virtual void affect() override;
	virtual void affect(const std::string &argument) override;


	const std::vector<std::string> &values() const;
};


}


#endif
