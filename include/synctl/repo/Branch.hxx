#ifndef _INCLUDE_SYNCTL_BRANCH_HXX_
#define _INCLUDE_SYNCTL_BRANCH_HXX_


#include <string>


namespace synctl {


class Branch
{
 public:
	struct Content
	{
		std::string  trunk;
		std::string  snapshot;
	};


 private:
	const std::string  _path;
	mutable Content    _content;
	mutable bool       _loaded = false;


 public:
	Branch(const std::string &path);

	void store(const Content &content);
	void load() const;

	const Content &content() const;
	const std::string &path() const noexcept;
};


}


#endif
