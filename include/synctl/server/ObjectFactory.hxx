#ifndef _INCLUDE_SYNCTL_SERVER_OBJECTFACTORY_HXX_
#define _INCLUDE_SYNCTL_SERVER_OBJECTFACTORY_HXX_


#include <memory>


namespace synctl {

class InputStream;
class Reference;

}


namespace synctl::server {


class Loader;
class Repository;
class Storer;


class ObjectFactory
{
	const Repository  *_repository = nullptr;


 public:
	ObjectFactory();
	ObjectFactory(const Repository *repository);

	std::unique_ptr<Storer> instance(synctl::InputStream *input) const;
	std::unique_ptr<Loader> instance(const synctl::Reference &ref) const;
};


}


#endif
