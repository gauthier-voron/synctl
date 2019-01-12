#ifndef _INCLUDE_SYNCTL_OPTION_HXX_
#define _INCLUDE_SYNCTL_OPTION_HXX_


#include <string>


namespace synctl {


class Option
{
	std::string  _longName;
	char         _shortName = '\0';
	size_t       _affected = 0;


 public:
	Option(const std::string &longName);
	Option(char shortName);
	Option(const std::string &longName, char shortName);
	virtual ~Option() = default;

	const std::string &longName() const;
	char shortName() const;

	virtual bool acceptArgument() const = 0;

	virtual void affect();
	virtual void affect(const std::string &argument) = 0;

	size_t &affected();
	const size_t &affected() const;
};


}


#endif
