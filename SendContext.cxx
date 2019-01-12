#include "synctl/SendContext.hxx"

#include <string>

#include "synctl/Filter.hxx"
#include "synctl/io/OutputStream.hxx"


using std::string;
using synctl::Filter;
using synctl::OutputStream;
using synctl::SendContext;


SendContext::SendContext()
{
}

SendContext::SendContext(const string &root, OutputStream *stream,
			 Filter *filter)
	: _root(root), _stream(stream), _filter(filter)
{
}

OutputStream *SendContext::stream()
{
	return _stream;
}

bool SendContext::contains(const string &path)
{
	return true;
}

const string &SendContext::root() const
{
	return _root;
}
