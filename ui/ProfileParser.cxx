#include "synctl/ui/ProfileParser.hxx"

#include <cctype>
#include <cstdint>
#include <string>

#include "synctl/io/FileInputStream.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/ui/Configuration.hxx"
#include "synctl/ui/Option.hxx"
#include "synctl/ui/ProfileSyntaxException.hxx"


using std::string;
using std::tolower;
using synctl::Configuration;
using synctl::FileInputStream;
using synctl::InputStream;
using synctl::Option;
using synctl::ProfileParser;
using synctl::ProfileSyntaxException;


bool ProfileParser::_readLine(InputStream *input, string *dest) const
{
	size_t count = 0;
	int16_t buf;
	char c;

	dest->clear();

	do {
		buf = input->read();
		if (buf < 0)
			return (count > 0);
		count += 1;

		c = (char) (buf & 0xff);
		dest->push_back(c);
	} while (c != '\n');

	return true;
}

bool ProfileParser::_parseLine(const string &line, string *key, string *value)
	const
{
	size_t i, st, en;

	for (i = 0; i < line.length(); i++)
		if ((line[i] != ' ') && (line[i] != '\t') && (line[i] != '\n'))
			break;

	if (i == line.length())
		return false;
	if (line[i] == '#')
		return false;

	st = en = i;
	for (; i < line.length(); i++) {
		if ((line[i] == '=') || (line[i] == '\n'))
			break;
		if ((line[i] != ' ') && (line[i] != '\t'))
			en = i;
	}

	if ((i == line.length()) || (line[i] != '='))
		throw i;

	*key = line.substr(st, en - st + 1);

	i += 1;
	for (; i < line.length(); i++)
		if ((line[i] != ' ') && (line[i] != '\t') && (line[i] != '\n'))
			break;

	if (i == line.length())
		throw i;

	st = en = i;
	for (; i < line.length(); i++) {
		if ((line[i] != ' ') && (line[i] != '\t') && (line[i] != '\n'))
			en = i;
	}

	if ((st == en) && ((en >= line.length()) || (line[en] == '\n')))
		throw st;

	*value = line.substr(st, en - st + 1);

	return true;
}

void ProfileParser::_affectOption(const string &key, const string &value)
{
	Option *option = nullptr;
	string lcval;
	size_t i;

	for (Option *o : _options)
		if (o->longName() == key) {
			option = o;
			break;
		}

	if (option == nullptr)
		return;
	if (option->affected())
		return;

	if (option->acceptArgument()) {
		option->affect(value);
		return;
	}

	for (i = 0; i < value.length(); i++)
		lcval.push_back(tolower(value[i]));

	if ((lcval == "yes") || (lcval == "y") || (lcval == "true") ||
	    (lcval == "t") || (lcval == "1")) {
		option->affect();
		return;
	}

	if ((lcval == "no") || (lcval == "n") || (lcval == "false") ||
	    (lcval == "f") || (lcval == "0") || (lcval == "nil")) {
		return;
	}

	throw value;
}

ProfileParser::ProfileParser(Configuration *configuration)
	: _configuration(configuration)
{
}

void ProfileParser::parse(const string &path)
{
	FileInputStream fis = FileInputStream(path);
	parse(&fis);
}

void ProfileParser::parse(InputStream *input)
{
	string line, key, value;
	size_t ln = 0;

	_options.clear();
	_configuration->getOptions(&_options);

	while (_readLine(input, &line)) {
		ln += 1;

		try {
			if (_parseLine(line, &key, &value) == false)
				continue;
		} catch (size_t column) {
			throw ProfileSyntaxException
				(line, ln, column + 1, line.substr(column));
		}

		try {
			_affectOption(key, value);
		} catch (string &value) {
			throw ProfileSyntaxException(line, ln, 0, value);
		}
	}
}
