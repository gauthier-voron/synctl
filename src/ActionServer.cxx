#include "synctl/ActionServer.hxx"

#include <string>
#include <vector>

#include "synctl/AdapterInputStream.hxx"
#include "synctl/AdapterOutputStream.hxx"
#include "synctl/Directory.hxx"
#include "synctl/OperandInvalidException.hxx"
#include "synctl/OperandMissingException.hxx"
#include "synctl/OperandUnexpectedException.hxx"
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
