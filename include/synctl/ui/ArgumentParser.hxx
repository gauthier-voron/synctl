#ifndef _INCLUDE_SYNCTL_ARGUMENTPARSER_HXX_
#define _INCLUDE_SYNCTL_ARGUMENTPARSER_HXX_


#include <string>
#include <vector>


namespace synctl {


class Configurationzzz;
class Option;


class ArgumentParser
{
	Configurationzzz       *_configuration;
	std::vector<Option *>   _options;
	bool                    _requireOrder = true;


	std::pair<bool, Option *>  _parseLongOption(const std::string &word);

	std::pair<bool, Option *>  _parseShortOptions(const std::string &word);

	std::pair<bool, Option *>  _parseOption(const std::string &word);


 public:
	ArgumentParser() = default;
	ArgumentParser(Configurationzzz *configuration);
	ArgumentParser(const ArgumentParser &other) = default;


	ArgumentParser &operator=(const ArgumentParser &other) = default;


	const bool &requireOrder() const noexcept;
	bool &requireOrder() noexcept;


	std::vector<std::string> parse(int argc, const char **argv);
	std::vector<std::string> parse(const std::vector<std::string> &args);
};


}


#endif
