#include "synctl/ui/ActionPush.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/io/Channel.hxx"
#include "synctl/plan/FirstMatchFilter.hxx"
#include "synctl/plan/GlobPattern.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/plan/ProtocolVersion.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/ui/OperandMissingException.hxx"
#include "synctl/ui/OperandUnexpectedException.hxx"
#include "synctl/ui/OptionLambda.hxx"
#include "synctl/ui/OptionString.hxx"


using std::make_unique;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::ActionPush;
using synctl::AdapterOutputStream;
using synctl::Channel;
using synctl::FirstMatchFilter;
using synctl::GlobPattern;
using synctl::OperandMissingException;
using synctl::OperandUnexpectedException;
using synctl::OptionLambda;
using synctl::OptionString;
using synctl::Protocol;
using synctl::Reference;


int ActionPush::_execute(const string &root, const string &server)
{
	Protocol::PushSettings psettings;
	unique_ptr<Protocol> protocol;
	unique_ptr<Channel> chan;

	chan = Channel::open(server);
	protocol = Protocol::clientHandcheck(chan.get());

	if (protocol == nullptr)
		return 1;

	psettings.localRoot = root;
	psettings.branchName = "Laurier";
	psettings.snapshotName = nullptr;
	psettings.filter = &_filter;
	protocol->push(psettings);

	protocol->exit();

	chan->close();

	return 0;
}

ActionPush::ActionPush()
	: Action("push")
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

int ActionPush::execute(const vector<string> &operands)
{
	if (operands.size() > 1)
		throw OperandUnexpectedException(operands[1]);
	if (_optionRoot.affected() == false)
		throw OperandMissingException(_optionRoot.longName());
	if (_optionServer.affected() == false)
		throw OperandMissingException(_optionServer.longName());

	return _execute(_optionRoot.value(), _optionServer.value());
}
