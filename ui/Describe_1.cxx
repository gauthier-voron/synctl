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


#include "synctl/plan/Describe_1.hxx"

#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>

#include <cstdint>
#include <string>

#include "synctl/io/OutputStream.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/repo/Snapshot.hxx"
#include "synctl/repo/Trunk.hxx"


using std::string;
using synctl::Describe_1;
using synctl::OutputStream;
using synctl::Repository;
using synctl::Snapshot;


static bool __match(const ::pcre2_code *code, const string &str)
{
	::pcre2_match_data *matcher;
	int ret;

	matcher = ::pcre2_match_data_create_from_pattern(code, NULL);

	ret = ::pcre2_match(code, (PCRE2_SPTR) str.c_str(),
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

void Describe_1::_ensureCompiled() const
{
	size_t erroff;
	int errnum;

	if (_trunkCode == nullptr) {
		_trunkCode = ::pcre2_compile
			((PCRE2_SPTR) _trunkRegex.c_str(),
			 PCRE2_ZERO_TERMINATED, 0, &errnum, &erroff, NULL);

		if (_trunkCode == nullptr)
			throw 0;
	}

	if (_branchRegex.empty())
		return;

	if (_branchCode == nullptr) {
		_branchCode = ::pcre2_compile
			((PCRE2_SPTR) _branchRegex.c_str(),
			 PCRE2_ZERO_TERMINATED, 0, &errnum, &erroff, NULL);

		if (_branchCode == nullptr)
			throw 0;
	}
}

void Describe_1::_describeBranches(OutputStream *output,
				   const Repository *repository) const
{
	const Snapshot *snapshot;
	const Trunk *trunk;
	uint64_t unixTime;

	output->writeInt(OP_LIST_1_BRANCH);

	for (const auto &pair : repository->branches()) {
		if (__match(_branchCode, pair.first) == false)
			continue;
		if (__match(_trunkCode, pair.second->content().trunk) == false)
			continue;

		trunk = repository->trunk(pair.second->content().trunk);

		if (trunk == nullptr)
			throw 0;

		snapshot = trunk->snapshot(pair.second->content().snapshot);

		if (snapshot == nullptr)
			throw 0;

		unixTime = snapshot->content().date;

		output->writeStr(pair.first);
		output->writeStr(pair.second->content().trunk);
		output->writeStr(pair.second->content().snapshot);
		output->writeInt(unixTime);
	}

	output->writeStr("");
}

void Describe_1::_describeTrunks(OutputStream *output,
				 const Repository *repository) const
{
	uint64_t unixTime;

	output->writeInt(OP_LIST_1_TRUNK);

	for (const auto &pair : repository->branches()) {
		if (__match(_trunkCode, pair.second->content().trunk) == false)
			continue;

		output->writeStr(pair.first);
		output->writeStr(pair.second->content().snapshot);
	}

	output->writeStr("");

	for (const auto &tpair : repository->trunks()) {
		if (__match(_trunkCode, tpair.first) == false)
			continue;

		output->writeStr(tpair.first);

		for (const auto &spair : tpair.second->snapshots()) {
			output->writeStr(spair.first);

			unixTime = spair.second->content().date;
			output->writeInt(unixTime);
		}

		output->writeStr("");
	}

	output->writeStr("");
}

Describe_1::~Describe_1()
{
	if (_trunkCode != nullptr)
		pcre2_code_free(_trunkCode);
	if (_branchCode != nullptr)
		pcre2_code_free(_branchCode);
}

string &Describe_1::trunkRegex()
{
	if (_trunkCode != nullptr)
		pcre2_code_free(_trunkCode);
	_trunkCode = nullptr;

	return _trunkRegex;
}

string &Describe_1::branchRegex()
{
	if (_branchCode != nullptr)
		pcre2_code_free(_branchCode);
	_branchCode = nullptr;

	return _branchRegex;
}

void Describe_1::describe(OutputStream *output, const Repository *repository)
	const
{
	_ensureCompiled();

	if (_branchCode != nullptr)
		_describeBranches(output, repository);
	else
		_describeTrunks(output, repository);
}
