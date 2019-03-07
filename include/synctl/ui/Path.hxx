#ifndef _INCLUDE_SYNCTL_PATH_HXX_
#define _INCLUDE_SYNCTL_PATH_HXX_


#include <string>


namespace synctl {


class Path
{
 public:
	static std::string resolvePath(const std::string &path);
};


}


#endif
