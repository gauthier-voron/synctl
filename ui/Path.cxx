#include "synctl/ui/Path.hxx"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "synctl/io/FSException.hxx"


using std::string;
using synctl::Path;


string Path::resolvePath(const string &path)
{
	struct passwd *pwd;
	size_t pos;

	if (path.empty() || (path[0] != '~'))
		return path;

	pos = path.find("/");
	if ((pos == 1) || (pos == string::npos))
		pwd = getpwuid(getuid());
	else
		pwd = getpwnam(path.substr(1, pos - 1).c_str());

	if (pwd == nullptr)
		throw FSException();

	return string(pwd->pw_dir) + path.substr(pos);
}
