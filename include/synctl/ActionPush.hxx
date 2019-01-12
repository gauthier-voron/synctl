#ifndef _INCLUDE_SYNCTL_ACTIONPUSH_HXX_
#define _INCLUDE_SYNCTL_ACTIONPUSH_HXX_


#include <string>
#include <vector>

#include "synctl/Action.hxx"
#include "synctl/OptionString.hxx"


namespace synctl {


class ActionPush : public Action
{
	OptionString  _optionRoot   = OptionString("root", 'r');
	OptionString  _optionServer = OptionString("server", 's');

	int _execute(const std::string &root, const std::string &server);


 public:
	ActionPush();

	virtual int execute(const std::vector<std::string> &operands);
};


}


#endif
