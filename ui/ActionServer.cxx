#include "synctl/ui/ActionServer.hxx"

#include <string>
#include <vector>

#include "synctl/io/Directory.hxx"
#include "synctl/io/FdInputStream.hxx"
#include "synctl/io/FdOutputStream.hxx"
#include "synctl/ui/OperandInvalidException.hxx"
#include "synctl/ui/OperandMissingException.hxx"
#include "synctl/ui/OperandUnexpectedException.hxx"
#include "synctl/io/PipeChannel.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/repo/Repository.hxx"


using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::ActionServer;
using synctl::Directory;
using synctl::FdInputStream;
using synctl::FdOutputStream;
using synctl::OperandInvalidException;
using synctl::OperandMissingException;
using synctl::OperandUnexpectedException;
using synctl::PipeChannel;
using synctl::Protocol;
using synctl::Repository;


int ActionServer::_execute(const string &serverPath)
{
	PipeChannel chan = PipeChannel(FdInputStream(0), FdOutputStream(1));
	unique_ptr<Protocol> protocol = Protocol::serverHandcheck(&chan);
	Repository repository = Repository(serverPath);

	if (protocol == nullptr)
		return 1;

	protocol->serve(&repository);

	chan.close();

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
