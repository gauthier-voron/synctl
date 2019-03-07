#include "synctl/ui/Command.hxx"

#include <string>
#include <vector>


using std::string;
using std::vector;
using synctl::Command;


void Command::shlex(const string &command, vector<string> *dest)
{
	size_t pos, start = 0;

	while ((pos = command.find(" ", start)) != string::npos) {
		if (pos != start) {
			dest->push_back(command.substr(start, pos - start));
			start = pos + 1;
		} else {
			start += 1;
		}
	}

	if (pos != start)
		dest->push_back(command.substr(start, pos - start));
}

vector<string> Command::shlex(const string &command)
{
	vector<string> ret;

	Command::shlex(command, &ret);
	return ret;
}
