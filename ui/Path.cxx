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


#include "synctl/ui/Path.hxx"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "synctl/io/FSException.hxx"


using std::string;
using synctl::Path;


string Path::resolvePath(const string &path)
{
	struct passwd *pwd;
	size_t pos;

	if (path.empty() || (path[0] != '~'))
		return path;

	pos = path.find("/");
	if ((pos == 1) || (pos == string::npos))
		pwd = getpwuid(getuid());
	else
		pwd = getpwnam(path.substr(1, pos - 1).c_str());

	if (pwd == nullptr)
		throw FSException();

	return string(pwd->pw_dir) + path.substr(pos);
}
