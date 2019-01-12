#ifndef _INCLUDE_SYNCTL_OBJECTSTORE_HXX_
#define _INCLUDE_SYNCTL_OBJECTSTORE_HXX_


#include <cstdint>
#include <memory>
#include <string>


namespace synctl {


class InputStream;
class OutputStream;
class Reference;
class TransientOutputStream;


class ObjectStore
{
	std::string  _path;


	bool _getReferencePath(const Reference &ref, std::string *dest) const;

	std::string _buildReferencePath(const Reference &ref);

	std::unique_ptr<InputStream>
	_readReferencePath(const Reference &reference) const;

	std::unique_ptr<OutputStream>
	_writeReferencePath(const Reference &reference);


 public:
	using Refcount = uint32_t;


	ObjectStore(const std::string &path);


	void initialize() const;


	std::unique_ptr<InputStream>
	readObject(const Reference &reference) const;

	Refcount readRefcount(const Reference &reference) const;

	void writeRefcount(const Reference &reference, Refcount cnt);


	void takeReference(const Reference &reference);


	std::unique_ptr<TransientOutputStream> newObject();

	std::unique_ptr<OutputStream>
	newObject(const Reference &reference);
};


}


#endif
