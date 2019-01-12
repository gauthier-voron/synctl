#include "synctl/ui/ActionServer.hxx"

#include <string>
#include <vector>

#include "synctl/io/AdapterInputStream.hxx"
#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/io/Directory.hxx"
#include "synctl/ui/OperandInvalidException.hxx"
#include "synctl/ui/OperandMissingException.hxx"
#include "synctl/ui/OperandUnexpectedException.hxx"
#include "synctl/Repository.hxx"


using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using synctl::ActionServer;
using synctl::AdapterInputStream;
using synctl::AdapterOutputStream;
using synctl::Directory;
using synctl::OperandInvalidException;
using synctl::OperandMissingException;
using synctl::OperandUnexpectedException;
using synctl::Repository;


int ActionServer::_execute(const string &serverPath)
{
	return 0;
}

ActionServer::ActionServer()
	: Action("server")
{
}

int ActionServer::execute(const vector<string> &operands)
{
	if (operands.size() < 2)
		throw OperandMissingException("server-path");
	if (operands.size() > 2)
		throw OperandUnexpectedException(operands[2]);

	return _execute(operands[1]);
}
