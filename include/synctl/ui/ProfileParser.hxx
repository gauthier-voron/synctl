#ifndef _INCLUDE_SYNCTL_PROFILEPARSER_HXX_
#define _INCLUDE_SYNCTL_PROFILEPARSER_HXX_


#include <string>
#include <vector>


namespace synctl {


class Configuration;
class InputStream;
class Option;


class ProfileParser
{
	Configuration       *_configuration;
	std::vector<Option *>   _options;


	bool _readLine(InputStream *input, std::string *dest) const;

	bool _parseLine(const std::string &line, std::string *key,
			std::string *value) const;

	void _affectOption(const std::string &key, const std::string &value);


 public:
	ProfileParser() = default;
	ProfileParser(Configuration *configuration);
	ProfileParser(const ProfileParser &other) = default;

	ProfileParser &operator=(const ProfileParser &other) = default;


	void parse(const std::string &path);
	void parse(InputStream *input);
};


}


#endif
