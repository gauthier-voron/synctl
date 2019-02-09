#include "synctl/ui/ActionPull.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/io/Channel.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/ui/OperandMissingException.hxx"
#include "synctl/ui/OperandUnexpectedException.hxx"


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


int ActionPull::_execute(const string &root, const string &server,
			 const string &reference)
{
	unique_ptr<Channel> chan = Channel::open(server);
	unique_ptr<Protocol> protocol = Protocol::clientHandcheck(chan.get());
	Reference ref = Reference::fromHex(reference);

	if (protocol == nullptr)
		return 1;


	protocol->pull(root, ref);
	protocol->exit();

	chan->close();

	return 0;
}

ActionPull::ActionPull()
	: Action("pull")
{
	addOption(&_optionReference);
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
	if (_optionReference.affected() == false)
		throw OperandMissingException(_optionReference.longName());

	return _execute(_optionRoot.value(), _optionServer.value(),
			_optionReference.value());
}
