#ifndef _INCLUDE_SYNCTL_ACTIONPULL_HXX_
#define _INCLUDE_SYNCTL_ACTIONPULL_HXX_


#include <memory>
#include <string>
#include <vector>

#include "synctl/io/Channel.hxx"
#include "synctl/tree/FirstMatchFilter.hxx"
#include "synctl/ui/Action.hxx"
#include "synctl/ui/OptionLambda.hxx"
#include "synctl/ui/OptionString.hxx"


namespace synctl {


class ActionPull : public Action
{
	FirstMatchFilter  _filter;
	OptionString      _optionCommand = OptionString("command", 'c');
	OptionLambda      _optionExclude;
	OptionLambda      _optionInclude;
	OptionString      _optionRoot    = OptionString("root", 'r');
	OptionString      _optionServer  = OptionString("server", 's');
	OptionString      _optionTrunk   = OptionString("trunk", 't');


	std::unique_ptr<Channel> _openChannel(const std::string &server);

	int _execute(const std::string &root, Channel *channel);


 public:
	ActionPull();

	virtual int execute(const std::vector<std::string> &operands);
};


}


#endif
