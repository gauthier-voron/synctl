#ifndef _INCLUDE_SYNCTL_TRUNKSTORE_HXX_
#define _INCLUDE_SYNCTL_TRUNKSTORE_HXX_


#include <map>
#include <memory>
#include <string>

#include "synctl/io/Directory.hxx"
#include "synctl/repo/Trunk.hxx"


namespace synctl {


class Trunk;


class TrunkStore
{
	Directory                                              _dir;
	mutable std::map<std::string, std::unique_ptr<Trunk>>  _trunks;
	mutable std::map<std::string, Trunk *>                 _rwptrs;
	mutable std::map<std::string, const Trunk *>           _roptrs;
	mutable bool                                           _loaded = false;


	void _ensureLoaded() const;


 public:
	TrunkStore(const std::string &path);


	void initialize() const;
	void load() const;


	Trunk *newTrunk(const std::string &name);

	Trunk *trunk(const std::string &name) noexcept;
	const Trunk *trunk(const std::string &name) const noexcept;


	const std::map<std::string, Trunk *> &trunks() noexcept;
	const std::map<std::string, const Trunk *> &trunks() const noexcept;
};


}


#endif
