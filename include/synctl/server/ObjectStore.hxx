#ifndef _INCLUDE_SYNCTL_SERVER_OBJECTSTORE_HXX_
#define _INCLUDE_SYNCTL_SERVER_OBJECTSTORE_HXX_


#include <cstdint>
#include <memory>
#include <string>


namespace synctl {

class InputStream;
class OutputStream;
class Reference;

}


namespace synctl::server {


class TransientOutputStream;


class ObjectStore
{
	std::string  _path;


	bool _getReferencePath(const synctl::Reference &ref, std::string *dest)
		const;

	std::string _buildReferencePath(const synctl::Reference &ref);

	std::unique_ptr<synctl::InputStream>
	_readReferencePath(const synctl::Reference &reference) const;

	std::unique_ptr<synctl::OutputStream>
	_writeReferencePath(const synctl::Reference &reference);


 public:
	using Refcount = uint32_t;


	ObjectStore(const std::string &path);


	void initialize() const;


	std::unique_ptr<synctl::InputStream>
	readObject(const synctl::Reference &reference) const;

	Refcount readRefcount(const synctl::Reference &reference) const;

	void writeRefcount(const synctl::Reference &reference, Refcount cnt);


	void takeReference(const synctl::Reference &reference);


	std::unique_ptr<TransientOutputStream> newObject();

	std::unique_ptr<synctl::OutputStream>
	newObject(const synctl::Reference &reference);
};


}


#endif
