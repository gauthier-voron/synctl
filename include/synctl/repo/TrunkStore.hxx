#ifndef _INCLUDE_SYNCTL_TRUNKSTORE_HXX_
#define _INCLUDE_SYNCTL_TRUNKSTORE_HXX_


#include <memory>
#include <string>
#include <vector>

#include "synctl/io/Directory.hxx"
#include "synctl/repo/Trunk.hxx"


namespace synctl {


class Trunk;


class TrunkStore
{
	Directory                                     _dir;
	mutable std::vector<std::unique_ptr<Trunk>>   _trunks;
	mutable std::vector<Trunk *>                  _rwptrs;
	mutable std::vector<const Trunk *>            _roptrs;
	mutable bool                                  _loaded = false;


	void _ensureLoaded() const;


 public:
	TrunkStore(const std::string &path);


	void initialize() const;
	void load() const;


	Trunk *newTrunk(const std::string &name);

	Trunk *trunk(const std::string &name) noexcept;
	const Trunk *trunk(const std::string &name) const noexcept;


	const std::vector<Trunk *> &trunks() noexcept;
	const std::vector<const Trunk *> &trunks() const noexcept;
};


}


#endif
