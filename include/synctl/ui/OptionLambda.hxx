#ifndef _INCLUDE_SYNCTL_OPTIONLAMBDA_HXX_
#define _INCLUDE_SYNCTL_OPTIONLAMBDA_HXX_


#include <functional>
#include <string>

#include "synctl/ui/Option.hxx"


namespace synctl {


class OptionLambda : public Option
{
	bool                                      _withArg;
	std::function<void(const std::string &)>  _lambdaWithArg;
	std::function<void()>                     _lambdaWithoutArg;


 public:
	OptionLambda(const std::string &longName,
		     std::function<void()> lambda);
	OptionLambda(char shortName, std::function<void()> lambda);
	OptionLambda(const std::string &longName, char shortName,
		     std::function<void()> lambda);

	OptionLambda(const std::string &longName,
		     std::function<void(const std::string &)> lambda);
	OptionLambda(char shortName,
		     std::function<void(const std::string &)> lambda);
	OptionLambda(const std::string &longName, char shortName,
		     std::function<void(const std::string &)> lambda);


	virtual bool acceptArgument() const override;

	virtual void affect() override;
	virtual void affect(const std::string &argument) override;
};


}


#endif
