#include "synctl/LoadContext.hxx"

#include <string>

#include "synctl/Filter.hxx"
#include "synctl/OutputStream.hxx"
#include "synctl/Repository.hxx"


using std::string;
using synctl::Filter;
using synctl::LoadContext;
using synctl::OutputStream;
using synctl::Repository;


LoadContext::LoadContext()
{
}

LoadContext::LoadContext(Repository *repository, OutputStream *stream,
			 Filter *filter)
	: _repository(repository), _stream(stream), _filter(filter)
{
}

OutputStream *LoadContext::stream()
{
	return _stream;
}

bool LoadContext::contains(const string &path)
{
	return true;
}

Repository *LoadContext::repository()
{
	return _repository;
}
