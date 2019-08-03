#ifndef _INCLUDE_SYNCTL_FORMATLIST_HXX_
#define _INCLUDE_SYNCTL_FORMATLIST_HXX_


#include <ctime>
#include <string>
#include <vector>


namespace synctl {


class FormatList
{
 public:
	virtual ~FormatList() = default;

	virtual void load(const std::string &trunk,
			  const std::string &snapshot,
			  const struct timespec &date,
			  const std::vector<std::string> &branches) = 0;

	virtual void flush() = 0;
};


}


#endif
