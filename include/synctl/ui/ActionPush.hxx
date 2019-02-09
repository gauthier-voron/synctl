#ifndef _INCLUDE_SYNCTL_ACTIONPUSH_HXX_
#define _INCLUDE_SYNCTL_ACTIONPUSH_HXX_


#include <string>
#include <vector>

#include "synctl/ui/Action.hxx"
#include "synctl/ui/OptionListString.hxx"
#include "synctl/ui/OptionString.hxx"


namespace synctl {


class ActionPush : public Action
{
	OptionListString  _optionExclude = OptionListString("exclude", 'e');
	OptionListString  _optionSkip    = OptionListString("skip", 'E');
	OptionListString  _optionInclude = OptionListString("include", 'i');
	OptionString      _optionRoot    = OptionString("root", 'r');
	OptionString      _optionServer  = OptionString("server", 's');

	int _execute(const std::string &root, const std::string &server);


 public:
	ActionPush();

	virtual int execute(const std::vector<std::string> &operands);
};


}


#endif
