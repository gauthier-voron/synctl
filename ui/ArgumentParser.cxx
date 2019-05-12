#include "synctl/ui/ArgumentParser.hxx"

#include <string>
#include <vector>

#include "synctl/ui/Configuration.hxx"
#include "synctl/ui/Option.hxx"
#include "synctl/ui/OptionUnknownException.hxx"
#include "synctl/ui/OptionWithArgumentException.hxx"
#include "synctl/ui/OptionWithoutArgumentException.hxx"


using std::make_pair;
using std::move;
using std::pair;
using std::string;
using std::vector;
using synctl::ArgumentParser;
using synctl::Configuration;
using synctl::Option;
using synctl::OptionUnknownException;
using synctl::OptionWithArgumentException;
using synctl::OptionWithoutArgumentException;


pair<bool, Option *> ArgumentParser::_parseLongOption(const string &word)
{
	Option *option = nullptr;
	string optname;
	size_t pos;

	pos = word.find('=');

	if (pos == 0)
		throw OptionUnknownException(word);
	else if (pos == string::npos)
		optname = word.substr(2);
	else
		optname = word.substr(2, pos - 2);

	for (Option *o : _options)
		if (o->longName() == optname) {
			option = o;
			break;
		}

	if (option == nullptr)
		throw OptionUnknownException(word);

	if (pos == string::npos) {
		if (option->acceptArgument())
			return make_pair<bool, Option *>(true, move(option));
		option->affect();
		return make_pair<bool, Option *>(true, nullptr);
	}

	if (option->acceptArgument()) {
		option->affect(word.substr(pos + 1));
		return make_pair<bool, Option *>(true, nullptr);
	}

	throw OptionWithoutArgumentException(word);
}

pair<bool, Option *> ArgumentParser::_parseShortOptions(const string &word)
{
	Option *option;
	size_t i;

	for (i = 1; i < word.length(); i++) {
		option = nullptr;
		for (Option *o : _options)
			if (o->shortName() == word[i]) {
				option = o;
				break;
			}

		if (option == nullptr)
			throw OptionUnknownException("-" + word.substr(i));

		if (option->acceptArgument() == false) {
			option->affect();
			continue;
		}

		if (i == (word.length() - 1))
			return make_pair<bool, Option *>(true, move(option));

		option->affect(word.substr(i + 1));
		break;
	}

	return make_pair<bool, Option *>(true, nullptr);
}

pair<bool, Option *> ArgumentParser::_parseOption(const string &word)
{
	if (word.length() < 2)
		return make_pair<bool, Option *>(false, nullptr);
	if (word[0] != '-')
		return make_pair<bool, Option *>(false, nullptr);

	if ((word.length() >= 3) && (word[1] == '-'))
		return _parseLongOption(word);
	return _parseShortOptions(word);
}

ArgumentParser::ArgumentParser(Configuration *configuration)
	: _configuration(configuration)
{
}

const bool &ArgumentParser::requireOrder() const noexcept
{
	return _requireOrder;
}

bool &ArgumentParser::requireOrder() noexcept
{
	return _requireOrder;
}

vector<string> ArgumentParser::parse(int argc, const char **argv)
{
	vector<string> args;
	int i;

	for (i = 0; i < argc; i++)
		args.push_back(argv[i]);

	return parse(args);
}

vector<string> ArgumentParser::parse(const vector<string> &args)
{
	pair<bool, Option *> p = make_pair<bool, Option *>(false, nullptr);
	vector<string> operands, ret;
	size_t i, token;

	_options.clear();
	_configuration->getOptions(&_options);

	for (i = 0; i < args.size(); i++) {
		if (p.second != nullptr) {
			p.second->affect(args[i]);
			p.second = nullptr;
			continue;
		}

		p = _parseOption(args[i]);

		if (p.first == false) {
			if (args[i] == "--") {
				i++;
				break;
			}

			if (_requireOrder)
				break;

			operands.push_back(args[i]);
			continue;
		}
	}

	if ((i == args.size()) && (p.second != nullptr))
		throw OptionWithArgumentException(args[i - 1]);

	for (; i < args.size(); i++)
		operands.push_back(args[i]);

	token = _configuration->getOperands(operands);
	for (i = token; i < operands.size(); i++)
		ret.push_back(operands[i]);

	return ret;
}
