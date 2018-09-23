#ifndef _INCLUDE_SYNCTL_SERVER_REPOSITORY_HXX_
#define _INCLUDE_SYNCTL_SERVER_REPOSITORY_HXX_


#include <cstdint>
#include <memory>
#include <string>

#include "synctl/server/BranchStore.hxx"
#include "synctl/server/ObjectStore.hxx"


namespace synctl {

class InputStream;
class OutputStream;
class Reference;

}


namespace synctl::server {


class Branch;
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


	void takeReference(const synctl::Reference &reference);

	std::unique_ptr<TransientOutputStream> newObject();


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
