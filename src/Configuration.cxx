#include "synctl/Configuration.hxx"

#include "synctl/Action.hxx"
#include "synctl/OperandInvalidException.hxx"
#include "synctl/OperandMissingException.hxx"
#include "synctl/Option.hxx"
#include "synctl/OptionBoolean.hxx"
#include "synctl/OptionString.hxx"
#include "synctl/OptionUnknownException.hxx"
#include "synctl/OptionWithArgumentException.hxx"
#include "synctl/OptionWithoutArgumentException.hxx"

#include <iostream>
#include <memory>
#include <string>
#include <vector>


using std::cout;
using std::endl;
using std::make_unique;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::Action;
using synctl::Configuration;
using synctl::OperandInvalidException;
using synctl::OperandMissingException;
using synctl::Option;
using synctl::OptionBoolean;
using synctl::OptionString;
using synctl::OptionUnknownException;
using synctl::OptionWithArgumentException;
using synctl::OptionWithoutArgumentException;


Configuration::Configuration()
{
}

Option *Configuration::_parseLongOption(const string &option, string *r)
{
	size_t pos;
	bool hasoptval;
	string optname, optarg;
	Option *found;

	pos = option.find_first_of('=');
	if (pos == string::npos) {
		optname = option.substr(2);
		hasoptval = false;
	} else {
		optname = option.substr(2, pos - 2);
		optarg = option.substr(pos + 1);
		hasoptval = true;
	}

	found = nullptr;
	for (auto &opt : _options) {
		if (opt->longName() != optname)
			continue;
		found = opt;
		break;
	}

	if (found == nullptr) {
		r->assign(option);
		return nullptr;
	} else {
		r->clear();
	}

	if (hasoptval == true) {
		if (found->acceptArgument() == false)
			throw OptionWithoutArgumentException(option);
		found->affect(optarg);
	} else if (found->acceptArgument() == true) {
		return found;
	} else {
		found->affect();
	}

	return nullptr;
}

Option *Configuration::_parseShortOptions(const string &options, string *rem)
{
	size_t pos;
	Option *found;
	string optarg;

	rem->clear();

	for (pos = 1; pos < options.length(); pos++) {
		found = nullptr;
		for (auto &opt : _options) {
			if (opt->shortName() != options[pos])
				continue;
			found = opt;
			break;
		}

		if (found == nullptr) {
			if (rem->empty())
				rem->push_back('-');
			rem->push_back(options[pos]);
			continue;
		}

		if (found->acceptArgument() == true) {
			if (pos == (options.length() - 1)) {
				return found;
			} else {
				optarg = options.substr(pos + 1);
				found->affect(optarg);
				break;
			}
		} else {
			found->affect();
		}
	}

	return nullptr;
}

void Configuration::_displayUsage()
{
	cout << "usage msg" << endl;
}

void Configuration::_displayVersion()
{
	cout << "version msg" << endl;
}

void Configuration::_loadCommandLine()
{
	vector<string> rem;
	bool eoo = false;
	Option *needarg = nullptr;
	string reject;

	for (const string &arg : _cmdline) {
		if (eoo == true) {
			rem.push_back(arg);
			continue;
		} else if (needarg != nullptr) {
			needarg->affect(arg);
			needarg = nullptr;
			continue;
		}

		if ((arg.length() < 2) || (arg[0] != '-')) {
			rem.push_back(arg);
			continue;
		} else if ((arg.length() == 2) && (arg[1] == '-')) {
			rem.push_back(arg);
			eoo = true;
			continue;
		}

		if (arg[1] == '-') {
			needarg = _parseLongOption(arg, &reject);
		} else {
			needarg = _parseShortOptions(arg, &reject);
		}

		if (reject.empty() == false)
			rem.push_back(reject);
	}

	if (needarg != nullptr) {
		if (needarg->longName() != "")
			throw OptionWithArgumentException
				("--" + needarg->longName());
		else
			throw OptionWithArgumentException
				("-" + string(needarg->shortName(), 1));
	}

	_cmdline = rem;
}

void Configuration::_loadConfigFile()
{
}

void Configuration::_load()
{
	_setupActions();
	_setupGlobalOptions();

	_loadCommandLine();

	if (_optionHelp.affected()) {
		_displayUsage();
		exit(0);
	} else if (_optionVersion.affected()) {
		_displayVersion();
		exit(0);
	}

	if (_cmdline.size() < 1)
		throw OperandMissingException("action");

	_action = nullptr;
	for (Action *action : _actions) {
		if (action->name() != _cmdline.front())
			continue;
		_action = action;
		break;
	}

	if (_action == nullptr)
		throw OperandInvalidException("action", _cmdline.front());
	
	_setupActionOptions(*_action);
	_loadCommandLine();

	_action->execute(_cmdline);
}

void Configuration::_setupActions()
{
	_actions.push_back(&_actionInit);
	_actions.push_back(&_actionPull);
	_actions.push_back(&_actionPush);
	_actions.push_back(&_actionServer);
}
	
void Configuration::_setupGlobalOptions()
{
	_options.clear();
	_options.push_back(&_optionConfig);
	_options.push_back(&_optionHelp);
	_options.push_back(&_optionVerbose);
	_options.push_back(&_optionVersion);
}

void Configuration::_setupActionOptions(const Action &action)
{
	_options.clear();

	for (Option *option : action.options())
		_options.push_back(option);
}

void Configuration::load(int argc, const char **argv)
{
	int i;

	_cmdline.clear();

	for (i = 1; i < argc; i++)
		_cmdline.push_back(string(argv[i]));

	_load();
	_cmdline.clear();
}

void Configuration::load(const vector<string> &args)
{
	_cmdline = args;
	_load();
}

const Action *Configuration::action() const noexcept
{
	return _action;
}
