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


#ifndef _INCLUDE_SYNCTL_XATTRIBUTE_HXX_
#define _INCLUDE_SYNCTL_XATTRIBUTE_HXX_


#include <map>
#include <string>


namespace synctl {


std::string getXattribute(const std::string &path, const std::string &name);

void setXattribute(const std::string &path, const std::string &name,
		   const std::string &value);


void getXattributes(const std::string &path,
		    std::map<std::string, std::string> *dest);

std::map<std::string, std::string> getXattributes(const std::string &path);

void setXattributes(const std::string &path,
		    const std::map<std::string, std::string> &attrs);


}


#endif
