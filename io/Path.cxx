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


#include "synctl/io/Path.hxx"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <random>
#include <string>

#include "synctl/io/FileOutputStream.hxx"
#include "synctl/io/IOException.hxx"


using std::move;
using std::mt19937;
using std::random_device;
using std::string;
using std::uniform_int_distribution;
using synctl::FileOutputStream;
using synctl::IOException;


string synctl::randomHexPath(const string &root, size_t len)
{
	random_device rd;
	mt19937 gen = mt19937(rd());
	uniform_int_distribution<uint64_t> dis;
	uint64_t number, digit;
	string path = root;
	struct stat st;
	size_t i, rlen;
	int ret;

	rlen = path.length();

	do {
		path.resize(rlen);
		number = dis(gen);
		for (i = 0; i < len; i++) {
			digit = number & 0xf;
			if (digit < 10)
				digit = digit + '0';
			else
				digit = digit - 10 + 'a';
			path.push_back(static_cast<char> (digit));
			number >>= 4;
		}

		ret = ::stat(path.c_str(), &st);
	} while (ret == 0);

	return path;
}

FileOutputStream synctl::randomHexOutput(string *path, size_t len)
{
	FileOutputStream output;
	string gen;

	do {
		gen = randomHexPath(*path, len);
		try {
			output = FileOutputStream(gen, O_EXCL);
			*path = move(gen);
			return output;
		} catch (IOException &) {
			// retry
		}
	} while (1);
}

void synctl::split(const string &path, string *prefix, string *suffix)
{
	size_t idx = path.length() - 1;
	size_t st;

	while ((idx > 0) && (path[idx] != '/'))
		idx--;

	st = idx;

	while ((idx > 0) && (path[idx] == '/'))
		idx--;

	if (prefix != nullptr)
		*prefix = path.substr(0, idx + 1);
	if (suffix != nullptr)
		*suffix = path.substr(st + 1);
}

string synctl::prefix(const string &path)
{
	string ret;

	split(path, &ret, nullptr);

	return ret;
}
