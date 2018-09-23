#include "synctl/client/SendContext.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/Filter.hxx"
#include "synctl/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/client/EntryFactory.hxx"
#include "synctl/client/Sender.hxx"


using std::string;
using std::vector;
using std::unique_ptr;
using synctl::Filter;
using synctl::client::EntryFactory;
using synctl::client::SendContext;


SendContext::SendContext(string *root, const string &path,
			 Filter *filter, Filter::Action action)
	: _root(root), _path(path), _filter(filter), _action(action)
{
}

SendContext::SendContext()
{
}

SendContext::SendContext(const string &root, Filter *filter)
	: _path(root), _filter(filter)
{
	_root = &_path;
}

bool SendContext::send(OutputStream *out, Reference *dest, const string &name)
{
	string spath;
	Filter::Action action;
	unique_ptr<Sender> sender;
	SendContext chcontext;

	if (_root != &_path)
		spath = _path;

	if (spath.empty() || (spath.back() != '/'))
		spath += '/';
	if (name != "/")
		spath += name;

	action = _filter->apply(spath);

	if (action == Filter::Action::Reject)
		return false;
	else if (action == Filter::Action::Ignore)
		action = _action;

	chcontext = SendContext(_root, spath, _filter, action);

	sender = EntryFactory().instance(chcontext.path());
	if (sender == nullptr)
		return false;

	if (action == Filter::Action::Accept) {
		return sender->send(out, dest, &chcontext);
	} else {
		return sender->traverse(out, dest, &chcontext);
	}
}

bool SendContext::send(OutputStream *out, Reference *dest, const char *name)
{
	return send(out, dest, string(name));
}

bool SendContext::send(OutputStream *out, Reference *dest)
{
	return send(out, dest, "/");
}

const string SendContext::path() const
{
	if (_root == &_path)
		return _path;
	return (*_root) + _path;
}

Filter::Action SendContext::action() const noexcept
{
	return _action;
}
