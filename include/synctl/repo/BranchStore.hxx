#ifndef _INCLUDE_SYNCTL_BRANCHSTORE_HXX_
#define _INCLUDE_SYNCTL_BRANCHSTORE_HXX_


#include <memory>
#include <string>
#include <vector>

#include "synctl/io/Directory.hxx"
#include "synctl/repo/Branch.hxx"


namespace synctl {


class BranchStore
{
	Directory                                     _dir;
	mutable std::vector<std::unique_ptr<Branch>>  _branches;
	mutable std::vector<Branch *>                 _rwptrs;
	mutable std::vector<const Branch *>           _roptrs;
	mutable bool                                  _loaded = false;


	void _ensureLoaded() const;


 public:
	BranchStore(const std::string &path);


	void initialize() const;
	void load() const;


	Branch *setBranch(const std::string &name,
			  const Branch::Content &content);

	Branch *branch(const std::string &name) noexcept;
	const Branch *branch(const std::string &name) const noexcept;
};


}


#endif
