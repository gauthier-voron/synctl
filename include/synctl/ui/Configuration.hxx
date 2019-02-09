#ifndef _INCLUDE_SYNCTL_CONFIGURATION_HXX_
#define _INCLUDE_SYNCTL_CONFIGURATION_HXX_


#include "synctl/ui/ActionInit.hxx"
#include "synctl/ui/ActionPull.hxx"
#include "synctl/ui/ActionPush.hxx"
#include "synctl/ui/ActionServer.hxx"
#include "synctl/ui/Option.hxx"
#include "synctl/ui/OptionBoolean.hxx"
#include "synctl/ui/OptionString.hxx"

#include <memory>
#include <string>
#include <vector>


namespace synctl {


class Configuration
{
	std::vector<Option *>     _options;
	std::vector<Action *>     _actions;
	std::vector<std::string>  _cmdline;
	std::string               _config;

	Action        *_action;
	ActionInit     _actionInit;
	ActionPull     _actionPull;
	ActionPush     _actionPush;
	ActionServer   _actionServer;

	OptionString   _optionConfig  = OptionString("C");
	OptionBoolean  _optionHelp    = OptionBoolean("help", 'h');
	OptionBoolean  _optionVerbose = OptionBoolean("verbose", 'v');
	OptionBoolean  _optionVersion = OptionBoolean("version", 'V');


	Option *_parseLongOption(const std::string &option, std::string *r);

	Option *_parseShortOptions(const std::string &option, std::string *r);


	void _displayUsage();

	void _displayVersion();


	// Parse the options / arguments present in the _cmdline vector.
	// If an option is not recognised, then it is considered as a boolean
	// option and kept for later.
	// The recognized options are removed from the command line.
	// Options of the command line override the options on the
	// configuration file.
	void _loadCommandLine();

	// Parse the configuration file at path _config.
	// If the file is not readable or contains syntax errors, throw an
	// exception of type (or subtype of) ConfigurationException.
	// The format of the configuration file is:
	//
	//   [ <action> ]
	//   <long-option-name> = <value>
	//   <long-boolean-option-name> = [ 'true' | 'yes' | 'false' | 'no' ]
	//
	// The action are the possible actions that can be specified on the
	// command line. The long option names are the option that can be
	// specified on the command line which have a long name.
	void _loadConfigFile();

	// Load the configuration from the command line stored in _cmdline.
	// If the command line specify a configuration file, also parse this
	// file and use the options inside as long as they do not conflict with
	// the command line options.
	// If the command line does not indicate a configuration file, try some
	// builtin paths.
	void _load();


	void _setupActions();
	void _setupGlobalOptions();
	void _setupActionOptions(const Action &action);


 public:
	Configuration();
	Configuration(const Configuration &other) = default;


	Configuration &operator=(const Configuration &other) = default;


	// Load the configuration given the provided command line.
	// Configuration parameters may as well be loaded from configuration
	// files listed in the command line or in the default values.
	// Signal configuration errors with exceptions derived from
	// ConfigException.
	void load(int argc, const char **argv);
	void load(const std::vector<std::string> &args);


	const Action *action() const noexcept;
};


}


#endif
