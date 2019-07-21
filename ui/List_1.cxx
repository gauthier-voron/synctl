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


#include "synctl/plan/List_1.hxx"

#include <cstdint>
#include <ctime>
#include <map>
#include <string>
#include <vector>

#include "synctl/io/InputStream.hxx"
#include "synctl/plan/Opcode.hxx"


using std::make_pair;
using std::map;
using std::move;
using std::string;
using std::vector;
using synctl::InputStream;
using synctl::List_1;


void List_1::_listBranch(InputStream *input, Handler handler)
{
	map<string, vector<string>> snapshotBranches;
	map<string, uint64_t> snapshotDates;
	map<string, string> snapshotTrunks;
	string branch, trunk, snapshot;
	struct timespec ts;
	uint64_t unixtime;

	branch = input->readStr();

	while (branch.empty() == false) {
		trunk = input->readStr();
		snapshot = input->readStr();
		unixtime = input->readInt<uint64_t>();

		auto pair = snapshotBranches.emplace
			(make_pair(snapshot, vector<string>()));

		pair.first->second.emplace_back(move(branch));

		snapshotDates[snapshot] = unixtime;
		snapshotTrunks[snapshot] = move(trunk);

		branch = input->readStr();
	}

	ts.tv_nsec = 0;

	for (const auto &p : snapshotBranches) {
		ts.tv_sec = snapshotDates[p.first] / 1000000000ul;
		ts.tv_nsec = snapshotDates[p.first] % 1000000000ul;
		handler(snapshotTrunks[p.first], p.first, ts, p.second);
	}
}

void List_1::_listTrunk(InputStream *input, Handler handler)
{
	map<string, vector<string>> snapshotBranches;
	string branch, snapshot, trunk;
	struct timespec ts;
	uint64_t unixtime;

	branch = input->readStr();

	while (branch.empty() == false) {
		snapshot = input->readStr();

		auto pair = snapshotBranches.emplace
			(make_pair(snapshot, vector<string>()));

		pair.first->second.emplace_back(move(branch));

		branch = input->readStr();
	}

	ts.tv_nsec = 0;

	trunk = input->readStr();

	while (trunk.empty() == false) {
		snapshot = input->readStr();

		while (snapshot.empty() == false) {
			unixtime = input->readInt<uint64_t>();

			ts.tv_sec = unixtime / 1000000000ul;
			ts.tv_nsec = unixtime % 1000000000ul;

			handler(trunk, snapshot, ts,
				snapshotBranches[snapshot]);

			snapshot = input->readStr();
		}

		trunk = input->readStr();
	}
}

void List_1::list(InputStream *input, Handler handler)
{
	opcode_t op = input->readInt<opcode_t>();

	if (op == OP_LIST_1_TRUNK)
		_listTrunk(input, handler);
	else if (op == OP_LIST_1_BRANCH)
		_listBranch(input, handler);
	else
		throw 0;
}
