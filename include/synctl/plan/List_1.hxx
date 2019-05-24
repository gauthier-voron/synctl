#ifndef _INCLUDE_SYNCTL_LIST_1_HXX_
#define _INCLUDE_SYNCTL_LIST_1_HXX_


#include <ctime>
#include <functional>
#include <string>
#include <vector>


namespace synctl {


class InputStream;


class List_1
{
 public:
	using Handler = std::function
		<void (const std::string &trunk,
		       const std::string &snapshot,
		       const struct timespec &date,
		       const std::vector<std::string> &branches)>;


 private:
	void _listBranch(InputStream *input, Handler handler);

	void _listTrunk(InputStream *input, Handler handler);


 public:
	void list(InputStream *input, Handler handler);
};


}


#endif
