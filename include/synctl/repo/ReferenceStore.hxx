#ifndef _INCLUDE_SYNCTL_REFERENCESTORE_HXX_
#define _INCLUDE_SYNCTL_REFERENCESTORE_HXX_


#include <cstdint>
#include <map>
#include <string>

#include "synctl/io/Directory.hxx"


namespace synctl {


class OutputStream;
class Reference;


using Refcount = uint32_t;


class ReferenceStore
{
	Directory                              _dir;
	mutable std::map<Reference, Refcount>  _refcounts;
	mutable bool                           _loaded = false;


	void _load() const;

	void _store() const;

	void _ensureLoaded() const;


 public:
	ReferenceStore(const std::string &path);
	~ReferenceStore();


	void initialize() const;


	Refcount readRefcount(const Reference &reference) const;

	void writeRefcount(const Reference &reference, Refcount cnt);


	void takeReference(const Reference &reference);

	void dumpReferences(OutputStream *output) const;
};


}


#endif
