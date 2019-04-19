#ifndef _INCLUDE_SYNCTL_PULL_1_HXX_
#define _INCLUDE_SYNCTL_PULL_1_HXX_


#include <string>

#include "synctl/io/LinkTracker.hxx"
#include "synctl/plan/Opcode.hxx"


namespace synctl {


class InputStream;


class Pull_1
{
	struct Context
	{
		std::string   apath;
		opcode_t      opcode;
		InputStream  *input;
		LinkTracker  *tracker;
	};


	void _delete(const std::string &path);
	void _delete(const Context *context);

	void _pullObject(const Context *context);

	void _createDirectory(const Context *context);

	void _mergeDirectory(const Context *context);

	void _pullDirectory(const Context *context);

	void _pullRegular(const Context *context);

	void _pullSymlink(const Context *context);

	void _pullLinks(const Context *context);

	void _pullLink(const Context *context);


 public:
	void pull(InputStream *input, const std::string &root);
};


}


#endif
