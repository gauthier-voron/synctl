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


#ifndef _INCLUDE_SYNCTL_LOGGERPUSH_1_HXX_
#define _INCLUDE_SYNCTL_LOGGERPUSH_1_HXX_


#include <string>


namespace synctl {


class LoggerPush_1
{
 public:
	virtual void receiveReference() = 0;

	virtual void seekEntry(const std::string &rpath, size_t len,
			       size_t total) = 0;

	virtual void sendEntry(const std::string &rpath, size_t len,
			       size_t total) = 0;

	virtual void createSnapshot(const std::string &trunk,
				    const std::string &snapshot,
				    const std::string &branch) = 0;
};


}


#endif
