#include "synctl/StoreContext.hxx"

#include "synctl/io/InputStream.hxx"
#include "synctl/Repository.hxx"


using synctl::InputStream;
using synctl::Repository;
using synctl::StoreContext;


StoreContext::StoreContext()
{
}

StoreContext::StoreContext(Repository *repository, InputStream *stream)
	: _repository(repository), _stream(stream)
{
}

InputStream *StoreContext::stream()
{
	return _stream;
}

Repository *StoreContext::repository()
{
	return _repository;
}
