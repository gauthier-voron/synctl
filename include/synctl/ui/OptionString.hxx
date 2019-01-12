#ifndef _INCLUDE_SYNCTL_OPTIONSTRING_HXX_
#define _INCLUDE_SYNCTL_OPTIONSTRING_HXX_


#include <string>

#include "synctl/ui/Option.hxx"


namespace synctl {


class OptionString : public Option
{
	std::string  _value;


 public:
	OptionString(const std::string &longName,
		     const std::string &defaultValue = "");
	OptionString(char shortName, const std::string &defaultValue = "");
	OptionString(const std::string &longName, char shortName,
		     const std::string &defaultValue = "");


	virtual bool acceptArgument() const;

	virtual void affect();
	virtual void affect(const std::string &argument);


	const std::string &value() const;
};


}


#endif
