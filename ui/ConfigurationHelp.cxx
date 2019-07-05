#include "synctl/ui/ConfigurationHelp.hxx"

#include <unistd.h>

#include <set>
#include <string>
#include <vector>

#include "synctl/ui/ArgumentParser.hxx"
#include "synctl/ui/OperandInvalidException.hxx"
#include "synctl/ui/OperandUnexpectedException.hxx"
#include "synctl/ui/OptionBoolean.hxx"


using std::set;
using std::string;
using std::to_string;
using std::vector;
using synctl::ArgumentParser;
using synctl::ConfigurationHelp;
using synctl::OperandInvalidException;
using synctl::OperandUnexpectedException;
using synctl::OptionBoolean;


ConfigurationHelp::ConfigurationHelp(ConfigurationBase *inner)
	: ConfigurationCommand(inner)
{
}

const OptionBoolean &ConfigurationHelp::optionProfile() const
{
	return _optionProfile;
}

const OptionBoolean &ConfigurationHelp::optionRegex() const
{
	return _optionRegex;
}

const OptionBoolean &ConfigurationHelp::optionTutorial() const
{
	return _optionTutorial;
}

bool ConfigurationHelp::hasCommand() const
{
	return _hasCommand;
}

size_t ConfigurationHelp::hasOptions() const
{
	size_t count = 0;

	if (regex())
		count += 1;
	if (tutorial())
		count += 1;

	return count;
}

bool ConfigurationHelp::profile() const
{
	return (_optionProfile.affected() > 0);
}

bool ConfigurationHelp::regex() const
{
	return (_optionRegex.affected() > 0);
}

bool ConfigurationHelp::tutorial() const
{
	return (_optionTutorial.affected() > 0);
}

const string &ConfigurationHelp::command() const
{
	return _operandCommand;
}

size_t ConfigurationHelp::getOperands(const vector<string> &args)
{
	if (args.size() < 1) {
		_hasCommand = false;
		return 0;
	}

	_hasCommand = true;
	_operandCommand = args[0];

	return 1;
}

void ConfigurationHelp::getOptions(vector<Option *> *dest)
{
	ConfigurationCommand::getOptions(dest);
	dest->push_back(&_optionProfile);
	dest->push_back(&_optionRegex);
	dest->push_back(&_optionTutorial);
}

static void __execMan(int section, const string &name)
{
	const char *argv[4];
	string strsec = to_string(section);
	int ret;

	argv[0] = "man";
	argv[1] = strsec.c_str();
	argv[2] = name.c_str();
	argv[3] = NULL;

	ret = ::execvp("man", (char *const *) argv);

	throw ret;
}

void ConfigurationHelp::displayProfileHelp()
{
	__execMan(5, "synctl-profile");
}

void ConfigurationHelp::displayRegexHelp()
{
	__execMan(7, "synctl-regex");
}

void ConfigurationHelp::displayTutorialHelp()
{
	__execMan(7, "synctl-tutorial");
}

void ConfigurationHelp::displayMainHelp()
{
	__execMan(1, "synctl");
}

void ConfigurationHelp::displayCommandHelp(const string &command)
{
	static set<string> availableCommands = {
		"help",
		"init",
		"list",
		"pull",
		"push",
		"server"
	};

	if (availableCommands.find(command) == availableCommands.end())
		throw OperandInvalidException("cmd", command);

	__execMan(1, "synctl-" + command);
}

static int __main(ConfigurationHelp *config)
{
	if (config->profile())
		config->displayProfileHelp();
	if (config->regex())
		config->displayRegexHelp();
	if (config->tutorial())
		config->displayTutorialHelp();

	if (config->hasCommand() == false)
		config->displayMainHelp();

	config->displayCommandHelp(config->command());

	return 1;
}

int ConfigurationHelp::main(ConfigurationBase *c, const vector<string> &args)
{
	ConfigurationHelp config = ConfigurationHelp(c);
	ArgumentParser parser = ArgumentParser(&config);
	vector<string> rem = parser.parse(args);

	if (config.help())
		ConfigurationHelp::displayCommandHelp(c->command());

	if (rem.empty() == false)
		throw OperandUnexpectedException(rem[0]);
	if (config.hasCommand() && config.hasOptions())
		throw OperandUnexpectedException(config.command());
	if (config.hasOptions() > 1)
		throw OperandUnexpectedException(args[1]);

	return __main(&config);
}
