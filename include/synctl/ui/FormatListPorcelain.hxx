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


#ifndef _INCLUDE_SYNCTL_FORMATLISTPORCELAIN_HXX_
#define _INCLUDE_SYNCTL_FORMATLISTPORCELAIN_HXX_


#include <ctime>
#include <map>
#include <string>
#include <vector>

#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/ui/FormatList.hxx"


namespace synctl {


class OutputStream;


class FormatListPorcelain : public FormatList {
	struct Date
	{
		struct timespec  ts;

		Date(const struct timespec &ts);

		bool operator<(const Date &other) const;
	};

	struct SnapshotInfo
	{
		std::string               trunk;
		std::string               name;
		std::vector<std::string>  branches;

		SnapshotInfo(const std::string &trunk, const std::string &name,
			     const std::vector<std::string> &branches);
	};


	AdapterOutputStream            _aoutput;
	OutputStream                  *_output;
	std::map<Date, SnapshotInfo>   _snapshots;


	void _format(const std::string &trunk, const std::string &snapshot,
		     const struct timespec &date,
		     const std::vector<std::string> &branches) const;


 public:
	FormatListPorcelain();
	FormatListPorcelain(OutputStream *output);
	FormatListPorcelain(const FormatListPorcelain &other) = delete;

	virtual void load(const std::string &trunk,
			  const std::string &snapshot,
			  const struct timespec &date,
			  const std::vector<std::string> &branches) override;

	virtual void flush() override;
};


}


#endif
