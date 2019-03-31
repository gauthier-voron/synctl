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
