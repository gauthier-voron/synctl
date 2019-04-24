#ifndef _INCLUDE_SYNCTL_REPOSITORY_HXX_
#define _INCLUDE_SYNCTL_REPOSITORY_HXX_


#include <cstdint>
#include <memory>
#include <string>

#include "synctl/repo/Branch.hxx"
#include "synctl/repo/BranchStore.hxx"
#include "synctl/repo/ObjectStore.hxx"
#include "synctl/repo/TrunkStore.hxx"


namespace synctl {


class Trunk;
class InputStream;
class OutputStream;
class Reference;
class TransientOutputStream;


class Repository
{
	std::string  _path;
	BranchStore  _bstore;
	TrunkStore   _tstore;
	ObjectStore  _ostore;


 public:
	Repository(const std::string &path);


	void initialize() const;


	void takeReference(const Reference &reference);

	void dumpReferences(OutputStream *output) const;


	std::unique_ptr<TransientOutputStream> newObject();

	std::unique_ptr<InputStream>
	readObject(const Reference &reference) const;

	size_t getObjectSize(const Reference &reference) const;


	Branch *setBranch(const std::string &name,
			  const Branch::Content &content);

	Branch *branch(const std::string &name);
	const Branch *branch(const std::string &name) const;


	Trunk *newTrunk(const std::string &name);

	Trunk *trunk(const std::string &name);
	const Trunk *trunk(const std::string &name) const;
};


}


#endif
