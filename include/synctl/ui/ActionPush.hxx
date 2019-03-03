#ifndef _INCLUDE_SYNCTL_ACTIONPUSH_HXX_
#define _INCLUDE_SYNCTL_ACTIONPUSH_HXX_


#include <string>
#include <vector>

#include "synctl/plan/FirstMatchFilter.hxx"
#include "synctl/ui/Action.hxx"
#include "synctl/ui/OptionLambda.hxx"
#include "synctl/ui/OptionString.hxx"


namespace synctl {


class ActionPush : public Action
{
	FirstMatchFilter  _filter;
	OptionLambda      _optionExclude;
	OptionLambda      _optionInclude;
	OptionString      _optionRoot    = OptionString("root", 'r');
	OptionString      _optionServer  = OptionString("server", 's');

	int _execute(const std::string &root, const std::string &server);


 public:
	ActionPush();

	virtual int execute(const std::vector<std::string> &operands);
};


}


#endif
