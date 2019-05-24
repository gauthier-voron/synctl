#ifndef _INCLUDE_SYNCTL_DESCRIBE_1_HXX_
#define _INCLUDE_SYNCTL_DESCRIBE_1_HXX_


#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>

#include <string>


namespace synctl {


class OutputStream;
class Repository;


class Describe_1
{
	std::string  _trunkRegex = ".*";
	std::string  _branchRegex = "";

	mutable ::pcre2_code  *_trunkCode = nullptr;
	mutable ::pcre2_code  *_branchCode = nullptr;


	void _ensureCompiled() const;

	void _describeBranches(OutputStream *output,
			       const Repository *repository) const;

	void _describeTrunks(OutputStream *output,
			     const Repository *repository) const;


 public:
	~Describe_1();

	std::string &trunkRegex();

	std::string &branchRegex();

	void describe(OutputStream *output, const Repository *repository)
		const;
};


}


#endif
