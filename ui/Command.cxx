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


#include "synctl/ui/Command.hxx"

#include <string>
#include <vector>


using std::string;
using std::vector;
using synctl::Command;


void Command::shlex(const string &command, vector<string> *dest)
{
	size_t pos, start = 0;

	while ((pos = command.find(" ", start)) != string::npos) {
		if (pos != start) {
			dest->push_back(command.substr(start, pos - start));
			start = pos + 1;
		} else {
			start += 1;
		}
	}

	if (pos != start)
		dest->push_back(command.substr(start, pos - start));
}

vector<string> Command::shlex(const string &command)
{
	vector<string> ret;

	Command::shlex(command, &ret);
	return ret;
}
