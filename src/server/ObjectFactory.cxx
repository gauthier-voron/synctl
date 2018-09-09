#include "synctl/server/ObjectFactory.hxx"

#include <memory>

#include "synctl/InputStream.hxx"
#include "synctl/IOException.hxx"
#include "synctl/Reference.hxx"
#include "synctl/Type.hxx"
#include "synctl/server/DirectoryV1.hxx"
#include "synctl/server/Loader.hxx"
#include "synctl/server/RegularV1.hxx"
#include "synctl/server/Repository.hxx"
#include "synctl/server/Storer.hxx"
#include "synctl/server/SymlinkV1.hxx"


using std::make_unique;
using std::unique_ptr;
using synctl::InputStream;
using synctl::IOException;
using synctl::Reference;
using synctl::Type;
using synctl::server::DirectoryV1;
using synctl::server::ObjectFactory;
using synctl::server::Loader;
using synctl::server::RegularV1;
using synctl::server::Repository;
using synctl::server::Storer;
using synctl::server::SymlinkV1;


ObjectFactory::ObjectFactory()
{
}

ObjectFactory::ObjectFactory(const Repository *repository)
	: _repository(repository)
{
}

unique_ptr<Storer> ObjectFactory::instance(InputStream *input) const
{
	Type type;

	input->readall(&type, sizeof (type));

	switch (type) {
	case Type::None:        return nullptr;
	case Type::DirectoryV1: return make_unique<DirectoryV1>();
	case Type::RegularV1:   return make_unique<RegularV1>();
	case Type::SymlinkV1:   return make_unique<SymlinkV1>();
	default:                throw IOException();
	}
}

unique_ptr<Loader> ObjectFactory::instance(const Reference &ref) const
{
	return nullptr;
}
