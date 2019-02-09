#ifndef _INCLUDE_SYNCTL_ACTIONPULL_HXX_
#define _INCLUDE_SYNCTL_ACTIONPULL_HXX_


#include <string>
#include <vector>

#include "synctl/ui/Action.hxx"
#include "synctl/ui/OptionString.hxx"


namespace synctl {


class ActionPull : public Action
{
	OptionString  _optionReference = OptionString("reference", 'R');
	OptionString  _optionRoot      = OptionString("root", 'r');
	OptionString  _optionServer    = OptionString("server", 's');

	int _execute(const std::string &root, const std::string &server,
		     const std::string &reference);


 public:
	ActionPull();

	virtual int execute(const std::vector<std::string> &operands);
};


}


#endif
