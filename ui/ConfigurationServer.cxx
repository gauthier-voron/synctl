#include "synctl/ui/ConfigurationServer.hxx"

#include <string>
#include <vector>

#include "synctl/io/Directory.hxx"
#include "synctl/io/FdInputStream.hxx"
#include "synctl/io/FdOutputStream.hxx"
#include "synctl/io/PipeChannel.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/ui/ArgumentParser.hxx"
#include "synctl/ui/ConfigurationBase.hxx"
#include "synctl/ui/ConfigurationHelp.hxx"
#include "synctl/ui/OperandInvalidException.hxx"
#include "synctl/ui/OperandMissingException.hxx"
#include "synctl/ui/OperandUnexpectedException.hxx"
#include "synctl/ui/Path.hxx"


using std::string;
using std::unique_ptr;
using std::vector;
using synctl::ArgumentParser;
using synctl::FdInputStream;
using synctl::FdOutputStream;
using synctl::ConfigurationBase;
using synctl::ConfigurationHelp;
using synctl::ConfigurationServer;
using synctl::OperandMissingException;
using synctl::OperandInvalidException;
using synctl::OperandUnexpectedException;
using synctl::Path;
using synctl::PipeChannel;
using synctl::Protocol;
using synctl::Repository;


ConfigurationServer::ConfigurationServer(ConfigurationBase *inner)
	: ConfigurationCommand(inner)
{
}

bool ConfigurationServer::hasServerPath() const
{
	return _hasOperand;
}

string ConfigurationServer::serverPath() const
{
	return Path::resolvePath(_operandServerPath);
}

size_t ConfigurationServer::getOperands(const vector<string> &args)
{
	if (args.size() < 1) {
		_hasOperand = false;
		return 0;
	}

	_hasOperand = true;
	_operandServerPath = args[0];

	return 1;
}

static int __main(ConfigurationServer *config)
{
	PipeChannel channel = PipeChannel(FdInputStream(0), FdOutputStream(1));
	unique_ptr<Protocol> protocol = Protocol::serverHandcheck(&channel);
	Repository repository = Repository(config->serverPath());

	if (protocol == nullptr)
		return 1;

	protocol->serve(&repository);

	channel.close();

	return 0;
}

int ConfigurationServer::main(ConfigurationBase *c, const vector<string> &args)
{
	ConfigurationServer config = ConfigurationServer(c);
	ArgumentParser parser = ArgumentParser(&config);
	vector<string> rem = parser.parse(args);

	if (config.help())
		ConfigurationHelp::displayCommandHelp(c->command());

	if (config.hasServerPath() == false)
		throw OperandMissingException("server-path");

	if (rem.empty() == false)
		throw OperandUnexpectedException(rem[0]);

	return __main(&config);
}
