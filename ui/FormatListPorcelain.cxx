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


#include "synctl/ui/FormatListPorcelain.hxx"

#include <cstdio>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/io/OutputStream.hxx"


#define DATE_LEN  30
#define NANO_LEN  20


using std::cout;
using std::make_pair;
using std::map;
using std::string;
using std::vector;
using synctl::AdapterOutputStream;
using synctl::FormatListPorcelain;
using synctl::OutputStream;


FormatListPorcelain::Date::Date(const struct timespec &_ts)
	: ts(_ts)
{
}

bool FormatListPorcelain::Date::operator<(const Date &other) const
{
	if (ts.tv_sec != other.ts.tv_sec)
		return (ts.tv_sec < other.ts.tv_sec);
	return (ts.tv_nsec < other.ts.tv_nsec);
}

FormatListPorcelain::SnapshotInfo::SnapshotInfo
(const string &_trunk, const string &_name, const vector<string> &_branches)
	: trunk(_trunk), name(_name), branches(_branches)
{
}

void FormatListPorcelain::_format(const string &trunk, const string &snapshot,
				  const struct timespec &date,
				  const vector<string> &branches) const
{
	char datebuf[DATE_LEN];
	struct tm datetm;
	string sep;

	::localtime_r(&date.tv_sec, &datetm);
	::strftime(datebuf, sizeof (datebuf), "%Y-%m-%d-%H-%M-%S-", &datetm);
	::snprintf(datebuf + NANO_LEN, sizeof (datebuf) - NANO_LEN,
		   "%09lu", date.tv_nsec);

	_output->write(trunk);
	_output->write(":");
	_output->write(snapshot);
	_output->write(":");
	_output->write(datebuf);
	_output->write(":");

	for (const string &b : branches) {
		_output->write(sep);
		_output->write(b);
		sep = ",";
	}

	_output->write("\n");
}

FormatListPorcelain::FormatListPorcelain()
	: _aoutput(cout), _output(&_aoutput)
{
}

FormatListPorcelain::FormatListPorcelain(OutputStream *output)
	: _output(output)
{
}

void FormatListPorcelain::load(const string &trunk, const string &snapshot,
			       const struct timespec &date,
			       const vector<string> &branches)
{
	_snapshots.emplace
		(make_pair(date, SnapshotInfo(trunk, snapshot, branches)));
}

void FormatListPorcelain::flush()
{
	for (auto it = _snapshots.rbegin(); it != _snapshots.rend(); ++it)
		_format(it->second.trunk, it->second.name, it->first.ts,
			it->second.branches);
}
