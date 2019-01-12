#include "synctl/ActionPull.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/AdapterOutputStream.hxx"
#include "synctl/DirectoryV1.hxx"
#include "synctl/OperandMissingException.hxx"
#include "synctl/OperandUnexpectedException.hxx"
#include "synctl/Reference.hxx"
#include "synctl/SendContext.hxx"


using std::string;
using std::unique_ptr;
using std::vector;
using synctl::ActionPull;
using synctl::AdapterOutputStream;
using synctl::DirectoryV1;
using synctl::OperandMissingException;
using synctl::OperandUnexpectedException;
using synctl::Reference;
using synctl::SendContext;


int ActionPull::_execute(const string &root, const string &server)
{
	return 0;
}

ActionPull::ActionPull()
	: Action("pull")
{
	addOption(&_optionRoot);
	addOption(&_optionServer);
}

int ActionPull::execute(const vector<string> &operands)
{
	if (operands.size() > 1)
		throw OperandUnexpectedException(operands[1]);
	if (_optionRoot.affected() == false)
		throw OperandMissingException(_optionRoot.longName());
	if (_optionServer.affected() == false)
		throw OperandMissingException(_optionServer.longName());

	return _execute(_optionRoot.value(), _optionServer.value());
}
