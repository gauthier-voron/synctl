#include "synctl/ui/ProfileSyntaxException.hxx"

#include <string>

#include "synctl/ui/ConfigurationException.hxx"


using std::string;
using synctl::ConfigurationException;
using synctl::ProfileSyntaxException;


ProfileSyntaxException::ProfileSyntaxException(const string &lineText,
					       size_t line, size_t column,
					       const string &what)
	: ConfigurationException(what), _lineText(lineText), _line(line)
	, _column(column)
{
}

const string &ProfileSyntaxException::lineText() const noexcept
{
	return _lineText;
}

size_t ProfileSyntaxException::line() const noexcept
{
	return _line;
}

size_t ProfileSyntaxException::column() const noexcept
{
	return _column;
}
