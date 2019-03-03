#include "synctl/ui/ActionPull.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/io/Channel.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/tree/FirstMatchFilter.hxx"
#include "synctl/tree/GlobPattern.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/ui/OperandMissingException.hxx"
#include "synctl/ui/OperandUnexpectedException.hxx"


using std::make_unique;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::ActionPull;
using synctl::AdapterOutputStream;
using synctl::Channel;
using synctl::OperandMissingException;
using synctl::OperandUnexpectedException;
using synctl::Protocol;
using synctl::Reference;


int ActionPull::_execute(const string &root, const string &server)
{
	unique_ptr<Channel> chan = Channel::open(server);
	unique_ptr<Protocol> protocol = Protocol::clientHandcheck(chan.get());
	Protocol::PullSettings psettings;

	if (protocol == nullptr)
		return 1;

	psettings.localRoot = root;
	psettings.branchName = "Laurier";
	psettings.snapshotName = "";
	psettings.filter = &_filter;
	protocol->pull(psettings);

	protocol->exit();

	chan->close();

	return 0;
}

ActionPull::ActionPull()
	: Action("pull")
	, _optionExclude("exclude", 'e', [&](auto ptrn) {
		_filter.append(make_unique<GlobPattern>(ptrn), Filter::Reject);
	  })
	, _optionInclude("include", 'i', [&](auto ptrn) {
	        _filter.append(make_unique<GlobPattern>(ptrn), Filter::Accept);
	  })
{
	addOption(&_optionExclude);
	addOption(&_optionInclude);
	addOption(&_optionRoot);
	addOption(&_optionServer);
}

int ActionPull::execute(const vector<string> &operands)
{
	if (operands.size() > 1)
		throw OperandUnexpectedException(operands[1]);
	if (_optionRoot.affected() == false)
		throw OperandMissingException(_optionRoot.longName());
	if (_optionServer.affected() == false)
		throw OperandMissingException(_optionServer.longName());

	return _execute(_optionRoot.value(), _optionServer.value());
}
