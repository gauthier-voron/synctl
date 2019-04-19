#include "synctl/ui/ActionPush.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/io/Channel.hxx"
#include "synctl/io/SshChannel.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/tree/FirstMatchFilter.hxx"
#include "synctl/tree/GlobPattern.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/ui/Command.hxx"
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
using synctl::Command;
using synctl::FirstMatchFilter;
using synctl::GlobPattern;
using synctl::OperandMissingException;
using synctl::OperandUnexpectedException;
using synctl::OptionLambda;
using synctl::OptionString;
using synctl::Protocol;
using synctl::Reference;
using synctl::SshChannel;


unique_ptr<Channel> ActionPush::_openChannel(const string &server)
{
	SshChannel::OpenSettings sshSettings;

	if (_optionCommand.affected()) {
		if (server.find("ssh://") != 0)
			throw OperandException(server);

		sshSettings.location = server.substr(6);
		sshSettings.remoteCommand =
			Command::shlex(_optionCommand.value());

		return SshChannel::open(sshSettings);
	}

	return Channel::open(server);
}

int ActionPush::_execute(const string &root, Channel *channel)
{
	Protocol::PushSettings psettings;
	unique_ptr<Protocol> protocol;

	protocol = Protocol::clientHandcheck(channel);

	if (protocol == nullptr)
		return 1;

	psettings.localRoot = root;
	psettings.trunkName = "master";
	psettings.snapshotName = nullptr;
	psettings.filter = &_filter;
	protocol->push(psettings);

	protocol->exit();

	channel->close();

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
	addOption(&_optionCommand);
	addOption(&_optionExclude);
	addOption(&_optionInclude);
	addOption(&_optionRoot);
	addOption(&_optionServer);
}

int ActionPush::execute(const vector<string> &operands)
{
	unique_ptr<Channel> channel;

	if (operands.size() > 1)
		throw OperandUnexpectedException(operands[1]);
	if (_optionRoot.affected() == false)
		throw OperandMissingException(_optionRoot.longName());
	if (_optionServer.affected() == false)
		throw OperandMissingException(_optionServer.longName());

	channel = _openChannel(_optionServer.value());

	return _execute(_optionRoot.value(), channel.get());
}
