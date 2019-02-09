#ifndef _INCLUDE_SYNCTL_ACTIONSERVER_HXX_
#define _INCLUDE_SYNCTL_ACTIONSERVER_HXX_


#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include "synctl/ui/Action.hxx"


namespace synctl {


class ActionServer : public Action
{
	int _execute(const std::string &serverPath);


 public:
	using Request = uint16_t;
	using Reply   = uint8_t;

	static const Request REQUEST_PUSH_V1         = 0x0001;

	static const Reply   REPLY_OK                = 0x00;
	static const Reply   REPLY_UNKNOWN_REQUEST   = 0x01;

	ActionServer();

	virtual int execute(const std::vector<std::string> &operands);
};


}


#endif
