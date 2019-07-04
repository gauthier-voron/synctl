#include "synctl/ui/ArgumentParser.hxx"
#include "synctl/ui/ConfigurationBase.hxx"
#include "synctl/ui/ConfigurationInit.hxx"
#include "synctl/ui/ConfigurationList.hxx"
#include "synctl/ui/ConfigurationPull.hxx"
#include "synctl/ui/ConfigurationPush.hxx"
#include "synctl/ui/ConfigurationServer.hxx"
#include "synctl/ui/OperandInvalidException.hxx"
#include "synctl/ui/OperandMissingException.hxx"

#include <map>
#include <string>
#include <vector>


using std::map;
using std::string;
using std::vector;
using synctl::ArgumentParser;
using synctl::ConfigurationBase;
using synctl::ConfigurationInit;
using synctl::ConfigurationList;
using synctl::ConfigurationPull;
using synctl::ConfigurationPush;
using synctl::ConfigurationServer;
using synctl::OperandInvalidException;
using synctl::OperandMissingException;


using MainFunction = int (*)(ConfigurationBase *, const vector<string> &);

static map<string, MainFunction> __commands {
	{ "init"   , ConfigurationInit::main   },
	{ "list"   , ConfigurationList::main   },
	{ "pull"   , ConfigurationPull::main   },
	{ "push"   , ConfigurationPush::main   },
	{ "server" , ConfigurationServer::main },
};


int main(int argc, const char **argv)
{
	ConfigurationBase base;
	ArgumentParser parser;
	vector<string> rem;
	string command;

	parser = ArgumentParser(&base);
	parser.requireOrder() = true;

	rem = parser.parse(argc - 1, argv + 1);

	if (base.hasCommand() == false)
		throw OperandMissingException("command");

	command = base.getCommand();

	auto it = __commands.find(command);

	if (it == __commands.end())
		throw OperandInvalidException("command", command);

	return it->second(&base, rem);
}
