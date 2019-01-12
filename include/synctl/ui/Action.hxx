#ifndef _INCLUDE_SYNCTL_ACTION_HXX_
#define _INCLUDE_SYNCTL_ACTION_HXX_


#include <string>
#include <vector>


namespace synctl {


class Option;


class Action
{
	std::string            _name;
	std::vector<Option *>  _options;


 protected:
	void addOption(Option *option);


 public:
	Action() = default;
	Action(const std::string &name);
	virtual ~Action() = default;

	virtual int execute(const std::vector<std::string> &operands) = 0;

	const std::string &name() const noexcept;
	const std::vector<Option *> &options() const noexcept;
};


}


#endif
