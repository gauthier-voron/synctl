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


#ifndef _INCLUDE_SYNCTL_GLOBPATTERN_HXX_
#define _INCLUDE_SYNCTL_GLOBPATTERN_HXX_


#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>

#include <string>

#include "synctl/tree/Pattern.hxx"


namespace synctl {


class GlobPattern : public Pattern
{
	::pcre2_code  *_pcreCode = nullptr;
	std::string    _perlRegex;


 public:
	GlobPattern();
	GlobPattern(const std::string &pattern);
	GlobPattern(const GlobPattern &other) = delete;
	GlobPattern(GlobPattern &&other);
	virtual ~GlobPattern();

	GlobPattern &operator=(const GlobPattern &other) = default;
	GlobPattern &operator=(GlobPattern &&other);

	virtual bool match(const std::string &path) override;

	virtual void write(OutputStream *output) const override;

	virtual void read(InputStream *input) override;
};


}


#endif
