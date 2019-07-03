#include "synctl/ui/ProfileSeeker.hxx"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <vector>

#include "synctl/ui/Path.hxx"


using std::string;
using std::vector;
using synctl::Path;
using synctl::ProfileSeeker;


void ProfileSeeker::_sanitize(std::string *path) const
{
	*path = Path::resolvePath(*path);
}

ProfileSeeker::ProfileSeeker(const vector<string> &paths)
{
	push(paths);
}

void ProfileSeeker::push(const string &paths)
{
	size_t end, start = 0;
	string path;

	do {
		end = paths.find(":", start);

		path = paths.substr(start, end);
		_sanitize(&path);
		_paths.push_back(path);

		start = end + 1;
	} while (end != string::npos);
}

void ProfileSeeker::push(const vector<string> &paths)
{
	for (const string &p : paths)
		push(p);
}

void ProfileSeeker::clear()
{
	_paths.clear();
}

bool ProfileSeeker::seek(const string &name, string *dest) const
{
	struct stat statbuf;
	string ppath;
	int ret;

	if (name.find("/") != string::npos) {
		*dest = name;
		return true;
	}

	for (const string &path : _paths) {
		ppath += path;
		ppath.push_back('/');
		ppath += name;

		ret = ::stat(ppath.c_str(), &statbuf);
		if ((ret == 0) && ((statbuf.st_mode & S_IFMT) == S_IFREG) &&
		    (access(ppath.c_str(), R_OK) == 0)) {
			*dest = ppath;
			return true;
		}

		ppath.clear();
	}

	return false;
}

string ProfileSeeker::seek(const string &name) const
{
	string ret;

	seek(name, &ret);

	return ret;
}

const vector<string> &ProfileSeeker::paths() const
{
	return _paths;
}
