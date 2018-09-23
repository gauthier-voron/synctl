#ifndef _INCLUDE_SYNCTL_FILTER_HXX_
#define _INCLUDE_SYNCTL_FILTER_HXX_


#include <string>


namespace synctl {


class Filter
{
 public:
	enum Action {
		Ignore,                          // Do the same than for parent
		Accept,                                    // Include the entry
		Reject,                // Exclude the entry and ignore children
		Traverse    // Check children and include if at least one is in
	};

	virtual ~Filter() = default;

	virtual Action apply(const std::string &path) = 0;
};


}


#endif
