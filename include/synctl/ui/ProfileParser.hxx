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
