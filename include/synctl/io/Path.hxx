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


#ifndef _INCLUDE_SYNCTL_PATH_HXX_
#define _INCLUDE_SYNCTL_PATH_HXX_


#include <string>

#include "synctl/io/FileOutputStream.hxx"


namespace synctl {


std::string randomHexPath(const std::string &root, size_t len);

FileOutputStream randomHexOutput(std::string *path, size_t len);

void split(const std::string &path, std::string *prefix, std::string *suffix);

std::string prefix(const std::string &path);


}


#endif
