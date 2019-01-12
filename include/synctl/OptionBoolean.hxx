#ifndef _INCLUDE_SYNCTL_OPTIONBOOLEAN_HXX_
#define _INCLUDE_SYNCTL_OPTIONBOOLEAN_HXX_


#include <string>

#include "synctl/Option.hxx"


namespace synctl {


class OptionBoolean : public Option
{
	bool _value = false;


 public:
	OptionBoolean(const std::string &longName);
	OptionBoolean(char shortName);
	OptionBoolean(const std::string &longName, char shortName);


	virtual bool acceptArgument() const;

	virtual void affect();
	virtual void affect(const std::string &argument);


	bool value() const;
};


}


#endif
