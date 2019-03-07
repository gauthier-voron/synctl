#include "synctl/io/SshChannel.hxx"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "synctl/io/Channel.hxx"
#include "synctl/io/PipeChannel.hxx"


using std::stoi;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;
using synctl::Channel;
using synctl::SshChannel;
using synctl::PipeChannel;


static void __parseLocation(string *user, string *address, uint16_t *port,
			    bool *home, string *homeUser, string *path,
			    const string &location)
{
	size_t sep, start = 0;
	size_t pos;
	int tmp;

	sep = location.find("@", start);
	if (sep == string::npos) {
		user->clear();
	} else {
		*user = location.substr(start, sep - start);
		start = sep + 1;
	}

	sep = location.find(":", start);
	if (sep == string::npos) {
		*port = 22;

		sep = location.find("/", start);
		if (sep == string::npos)
			throw 0;

		*address = location.substr(start, sep - start);
		start = sep + 1;
	} else {
		*address = location.substr(start, sep - start);
		start = sep + 1;

		sep = location.find("/", start);
		if (sep == string::npos)
			throw 0;

		tmp = stoi(location.substr(start, sep - start), &pos, 10);
		if (pos != (sep - start))
			throw 0;
		if (tmp > 65535)
			throw 0;

		*port = (uint16_t) tmp;
		start = sep + 1;
	}

	if ((location.length() > start) && (location[start] == '~')) {
		*home = true;

		start += 1;
		sep = location.find("/", start);

		if ((sep == string::npos) || (sep == start)) {
			homeUser->clear();
			start += 1;
		} else {
			*homeUser = location.substr(start, sep - start);
			start = sep + 1;
		}
	} else {
		*home = false;
		homeUser->clear();
	}

	if (start < location.length())
		*path = location.substr(start);
	else
		path->clear();
}

unique_ptr<Channel> SshChannel::open(const OpenSettings &settings)
{
	string user, address, path, homeUser;
	vector<string> command;
	uint16_t port;
	bool home;

	__parseLocation(&user, &address, &port, &home, &homeUser, &path,
			settings.location);

	if (user.empty() == false)
		address = user + "@" + address;

	if (home)
		path = "~" + homeUser + "/" + path;
	else
		path = "/" + path;

	command.push_back("ssh");
	command.push_back("-p");
	command.push_back(to_string(port));

	command.push_back(address);

	if (settings.remoteCommand.empty()) {
		command.push_back("synctl");
		command.push_back("server");
	} else {
		for (const string &s : settings.remoteCommand)
			command.push_back(s);
	}

	command.push_back(path);

	return PipeChannel::open(command);
}
