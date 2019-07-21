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


#include "synctl/plan/LoggerPush0_1.hxx"

#include <string>


using std::string;
using synctl::LoggerPush0_1;


void LoggerPush0_1::receiveReference()
{
}

void LoggerPush0_1::seekEntry(const string &rpath [[gnu::unused]],
			      size_t len [[gnu::unused]],
			      size_t total [[gnu::unused]])
{
}

void LoggerPush0_1::sendEntry(const string &rpath [[gnu::unused]],
			      size_t len [[gnu::unused]],
			      size_t total [[gnu::unused]])
{
}

void LoggerPush0_1::createSnapshot(const string &trunk [[gnu::unused]],
				   const string &snapshot [[gnu::unused]],
				   const string &branch [[gnu::unused]])
{
}
