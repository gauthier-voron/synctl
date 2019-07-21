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


#include "synctl/ui/ArgumentParser.hxx"
#include "synctl/ui/ConfigurationBase.hxx"
#include "synctl/ui/ConfigurationHelp.hxx"
#include "synctl/ui/ConfigurationInit.hxx"
#include "synctl/ui/ConfigurationList.hxx"
#include "synctl/ui/ConfigurationPull.hxx"
#include "synctl/ui/ConfigurationPush.hxx"
#include "synctl/ui/ConfigurationServer.hxx"
#include "synctl/ui/OperandInvalidException.hxx"
#include "synctl/ui/OperandMissingException.hxx"

#include <map>
#include <string>
#include <vector>


using std::map;
using std::string;
using std::vector;
using synctl::ArgumentParser;
using synctl::ConfigurationBase;
using synctl::ConfigurationHelp;
using synctl::ConfigurationInit;
using synctl::ConfigurationList;
using synctl::ConfigurationPull;
using synctl::ConfigurationPush;
using synctl::ConfigurationServer;
using synctl::OperandInvalidException;
using synctl::OperandMissingException;


using MainFunction = int (*)(ConfigurationBase *, const vector<string> &);

static map<string, MainFunction> __commands {
	{ "help"   , ConfigurationHelp::main   },
	{ "init"   , ConfigurationInit::main   },
	{ "list"   , ConfigurationList::main   },
	{ "pull"   , ConfigurationPull::main   },
	{ "push"   , ConfigurationPush::main   },
	{ "server" , ConfigurationServer::main },
};


int main(int argc, const char **argv)
{
	ConfigurationBase base;
	ArgumentParser parser;
	vector<string> rem;
	string command;

	parser = ArgumentParser(&base);
	parser.requireOrder() = true;

	rem = parser.parse(argc - 1, argv + 1);

	if (base.help()) {
		if (base.hasCommand())
			ConfigurationHelp::displayCommandHelp(base.command());
		else
			ConfigurationHelp::displayMainHelp();
	}

	if (base.hasCommand() == false)
		throw OperandMissingException("command");

	command = base.command();

	auto it = __commands.find(command);

	if (it == __commands.end())
		throw OperandInvalidException("command", command);

	return it->second(&base, rem);
}
