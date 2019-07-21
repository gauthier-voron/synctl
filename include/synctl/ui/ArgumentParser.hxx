// Copyright 2019 Gauthier Voron
//
// This file is part of Synctl
//
// Synctl is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Fundation, either version 3 of the License, or (at your option) any later
// version.
//
// Synctl is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANDABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// Synctl. If not, see <https://www.gnu.org/licenses/>.
//


#ifndef _INCLUDE_SYNCTL_ARGUMENTPARSER_HXX_
#define _INCLUDE_SYNCTL_ARGUMENTPARSER_HXX_


#include <string>
#include <vector>


namespace synctl {


class Configuration;
class Option;


class ArgumentParser
{
	Configuration       *_configuration;
	std::vector<Option *>   _options;
	bool                    _requireOrder = true;


	std::pair<bool, Option *>  _parseLongOption(const std::string &word);

	std::pair<bool, Option *>  _parseShortOptions(const std::string &word);

	std::pair<bool, Option *>  _parseOption(const std::string &word);


 public:
	ArgumentParser() = default;
	ArgumentParser(Configuration *configuration);
	ArgumentParser(const ArgumentParser &other) = default;


	ArgumentParser &operator=(const ArgumentParser &other) = default;


	const bool &requireOrder() const noexcept;
	bool &requireOrder() noexcept;


	std::vector<std::string> parse(int argc, const char **argv);
	std::vector<std::string> parse(const std::vector<std::string> &args);
};


}


#endif
