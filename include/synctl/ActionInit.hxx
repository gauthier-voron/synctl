#ifndef _INCLUDE_SYNCTL_ACTIONINIT_HXX_
#define _INCLUDE_SYNCTL_ACTIONINIT_HXX_


#include <string>
#include <vector>

#include "synctl/Action.hxx"
#include "synctl/OptionBoolean.hxx"


namespace synctl {


class ActionInit : public Action
{
	OptionBoolean  _optionForce = OptionBoolean("force", 'f');

	int _execute(const std::string &serverPath);


 public:
	ActionInit();

	virtual int execute(const std::vector<std::string> &operands);
};


}


#endif
