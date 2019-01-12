#include "synctl/ReceiveContext.hxx"

#include <string>

#include "synctl/Filter.hxx"
#include "synctl/io/InputStream.hxx"


using std::string;
using synctl::Filter;
using synctl::InputStream;
using synctl::ReceiveContext;


ReceiveContext::ReceiveContext()
{
}

ReceiveContext::ReceiveContext(const string &root, InputStream *stream)
	: _root(root), _stream(stream)
{
}

InputStream *ReceiveContext::stream()
{
	return _stream;
}

const string &ReceiveContext::root() const
{
	return _root;
}
