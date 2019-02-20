#ifndef _INCLUDE_SYNCTL_REPOSITORY_HXX_
#define _INCLUDE_SYNCTL_REPOSITORY_HXX_


#include <cstdint>
#include <memory>
#include <string>

#include "synctl/repo/BranchStore.hxx"
#include "synctl/repo/ObjectStore.hxx"


namespace synctl {


class Branch;
class InputStream;
class OutputStream;
class Reference;
class TransientOutputStream;


class Repository
{
	std::string  _path;
	BranchStore  _bstore;
	ObjectStore  _ostore;


	// std::unique_ptr<synctl::InputStream>
	// _readObject(const synctl::Reference &reference) const;

	// Refcount _readRefcount(const synctl::Reference &reference) const;

	// std::unique_ptr<synctl::OutputStream>
	// _writeObject(const synctl::Reference &reference);

	// void _writeRefcount(const synctl::Reference &reference, Refcount cnt);


 public:
	Repository(const std::string &path);


	void initialize() const;


	void takeReference(const Reference &reference);

	void dumpReferences(OutputStream *output) const;


	std::unique_ptr<TransientOutputStream> newObject();

	std::unique_ptr<InputStream>
	readObject(const Reference &reference) const;

	size_t getObjectSize(const Reference &reference) const;


	Branch *newBranch(const std::string &name);

	Branch *branch(const std::string &name);
	const Branch *branch(const std::string &name) const;

	
	// const RepositorySnapshot &getTrunk() const;
	// void setTrunk(const RepositorySnapshot &trunk);

	// void dumpReflist(std::ostream &os);

	// NewRepositoryObject newObject();
	// RepositoryObject getObject(const synctl::Reference &ref) const;
	// void delObject(const synctl::Reference &ref);
};


}


#endif
