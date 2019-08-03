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


#ifndef _INCLUDE_SYNCTL_FIRSTMATCHFILTER_HXX_
#define _INCLUDE_SYNCTL_FIRSTMATCHFILTER_HXX_


#include <memory>
#include <string>
#include <vector>

#include "synctl/tree/Filter.hxx"


namespace synctl {


class InputStream;
class OutputStream;
class Pattern;


class FirstMatchFilter : public Filter
{
	using pattern_ptr = std::unique_ptr<Pattern>;
	using rule_t = std::pair<pattern_ptr, Filter::Action>;

	std::vector<rule_t>  _rules;


 public:
	FirstMatchFilter();
	FirstMatchFilter(const FirstMatchFilter &other) = default;
	virtual ~FirstMatchFilter();

	FirstMatchFilter &operator=(const FirstMatchFilter &other) = default;


	void append(std::unique_ptr<Pattern> pattern, Filter::Action action);

	virtual Filter::Action apply(const std::string &path) override;

	virtual void write(OutputStream *output) const override;

	virtual void read(InputStream *input) override;
};


}


#endif
