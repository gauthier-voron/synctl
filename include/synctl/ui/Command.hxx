#ifndef _INCLUDE_SYNCTL_COMMAND_HXX_
#define _INCLUDE_SYNCTL_COMMAND_HXX_


#include <string>
#include <vector>


namespace synctl {


class Command
{
 public:
	static void shlex(const std::string &command,
			  std::vector<std::string> *dest);

	static std::vector<std::string> shlex(const std::string &command);
};


}


#endif
