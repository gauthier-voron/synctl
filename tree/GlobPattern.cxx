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


#include "synctl/tree/GlobPattern.hxx"

#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>

#include <string>

#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"


using std::move;
using std::string;
using synctl::GlobPattern;
using synctl::InputStream;
using synctl::OutputStream;


string __globToRegex(const string &glob)
{
	size_t i, j, len = glob.length();
	string escape = "^\\(){}.+";
	bool xwcard = false;
	bool wwcard = false;
	bool slash = false;
	bool wcard = false;
	string ret;

	for (i = 0; i < len; i++) {
		if (xwcard)
			goto flush;
		else if (wwcard) {
			if (glob[i] == '/') {
				xwcard = true;
				goto next;
			}
			goto flush;
		} else if (wcard) {
			if (glob[i] == '*') {
				wwcard = true;
				goto next;
			}
			goto flush;
		} else if (slash) {
			if (glob[i] == '*') {
				wcard = true;
				goto next;
			}
			goto flush;
		}

		if (glob[i] == '/') {
			slash = true;
			goto next;
		} else if (glob[i] == '*') {
			wcard = true;
			goto next;
		} else if (glob[i] == '?') {
			ret.append("[^/]");
			goto next;
		}

		for (j = 0; j < escape.length(); j++) {
			if (glob[i] == escape[j]) {
				ret.push_back('\\');
				ret.push_back(escape[j]);
				goto next;
			}
		}

		ret.push_back(glob[i]);

	next:
		continue;

	flush:
		if ((slash || wwcard || xwcard) && ret.empty())
			ret.push_back('^');
		if (slash)
			ret.push_back('/');
		if (xwcard)
			ret.append("(.+/)*");
		else if (wwcard)
			ret.append(".*");
		else if (wcard)
			ret.append("[^/]*");

		slash = false;
		wcard = false;
		wwcard = false;
		xwcard = false;

		i--;
	}

	if (slash || wcard || wwcard || xwcard)
		goto flush;

	ret.push_back('$');

	return ret;
}

GlobPattern::GlobPattern()
{
}

GlobPattern::GlobPattern(const string &pattern)
{
	size_t erroff;
	int errnum;

	_perlRegex = __globToRegex(pattern);
	_pcreCode = ::pcre2_compile((PCRE2_SPTR) _perlRegex.c_str(),
				    PCRE2_ZERO_TERMINATED, 0, &errnum, &erroff,
				    NULL);

	if (_pcreCode == nullptr)
		throw 0;
}

GlobPattern::GlobPattern(GlobPattern &&other)
	: _pcreCode(other._pcreCode), _perlRegex(move(other._perlRegex))
{
	other._pcreCode = nullptr;
}

GlobPattern::~GlobPattern()
{
	if (_pcreCode != nullptr)
		pcre2_code_free(_pcreCode);
}


GlobPattern &GlobPattern::operator=(GlobPattern &&other)
{
	if (_pcreCode != nullptr)
		pcre2_code_free(_pcreCode);

	_perlRegex = move(other._perlRegex);

	_pcreCode = other._pcreCode;
	other._pcreCode = nullptr;

	return *this;
}

bool GlobPattern::match(const string &path)
{
	::pcre2_match_data *matcher;
	int ret;

	matcher = ::pcre2_match_data_create_from_pattern(_pcreCode, NULL);

	ret = ::pcre2_match(_pcreCode, (PCRE2_SPTR) path.c_str(),
			    PCRE2_ZERO_TERMINATED, 0, 0, matcher, NULL);

	pcre2_match_data_free(matcher);

	if (ret == PCRE2_ERROR_NOMATCH) {
		return false;
	} else if (ret >= 0) {
		return true;
	} else {
		throw 0;
	}
}

void GlobPattern::write(OutputStream *output) const
{
	output->writeStr(_perlRegex);
}

void GlobPattern::read(InputStream *input)
{
	size_t erroff;
	int errnum;

	_perlRegex = input->readStr();

	if (_pcreCode != NULL)
		pcre2_code_free(_pcreCode);

	_pcreCode = ::pcre2_compile((PCRE2_SPTR) _perlRegex.c_str(),
				    PCRE2_ZERO_TERMINATED, 0, &errnum, &erroff,
				    NULL);

	if (_pcreCode == NULL)
		throw 0;
}
