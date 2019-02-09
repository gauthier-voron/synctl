#include "synctl/ui/ActionPush.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/io/Channel.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/plan/ProtocolVersion.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/ui/OperandMissingException.hxx"
#include "synctl/ui/OperandUnexpectedException.hxx"


using std::string;
using std::unique_ptr;
using std::vector;
using synctl::ActionPush;
using synctl::AdapterOutputStream;
using synctl::Channel;
using synctl::OperandMissingException;
using synctl::OperandUnexpectedException;
using synctl::Protocol;
using synctl::Reference;


int ActionPush::_execute(const string &root, const string &server)
{
	unique_ptr<Channel> chan = Channel::open(server);
	unique_ptr<Protocol> protocol = Protocol::clientHandcheck(chan.get());
	Protocol::PushSettings psettings;

	if (protocol == nullptr)
		return 1;

	psettings.localRoot = root;
	psettings.branchName = "Laurier";
	psettings.snapshotReference = nullptr;
	protocol->push(psettings);

	protocol->exit();

	chan->close();

	return 0;
}

ActionPush::ActionPush()
	: Action("push")
{
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
