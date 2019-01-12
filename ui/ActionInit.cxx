#include "synctl/ui/ActionInit.hxx"

#include <string>
#include <vector>

#include "synctl/io/Directory.hxx"
#include "synctl/io/IOException.hxx"
#include "synctl/ui/OperandInvalidException.hxx"
#include "synctl/ui/OperandMissingException.hxx"
#include "synctl/ui/OperandUnexpectedException.hxx"
#include "synctl/Repository.hxx"


using std::string;
using std::vector;
using synctl::ActionInit;
using synctl::Directory;
using synctl::IOException;
using synctl::OperandInvalidException;
using synctl::OperandMissingException;
using synctl::OperandUnexpectedException;
using synctl::Repository;


int ActionInit::_execute(const string &serverPath)
{
	Repository repository = Repository(serverPath);
	Directory directory = Directory(serverPath);

	if (directory.exists()) {
		if (_optionForce.value() == true)
			directory.remove();
		else
			throw OperandInvalidException("server-path",
						      serverPath);
	}

	repository.initialize();

	return 0;
}

ActionInit::ActionInit()
	: Action("init")
{
	addOption(&_optionForce);
}

int ActionInit::execute(const vector<string> &operands)
{
	if (operands.size() < 2)
		throw OperandMissingException("server-path");
	if (operands.size() > 2)
		throw OperandUnexpectedException(operands[2]);

	return _execute(operands[1]);
}
