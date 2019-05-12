#ifndef _INCLUDE_SYNCTL_OPERANDINVALIDEXCEPTION_HXX_
#define _INCLUDE_SYNCTL_OPERANDINVALIDEXCEPTION_HXX_


#include <string>

#include "synctl/ui/ConfigurationException.hxx"


namespace synctl {


class ProfileSyntaxException : public ConfigurationException
{
	std::string  _lineText;
	size_t       _line;
	size_t       _column;


 public:
	ProfileSyntaxException() = default;
	ProfileSyntaxException(const std::string &lineText,
			       size_t line, size_t column,
			       const std::string &what);


	const std::string &lineText() const noexcept;

	size_t line() const noexcept;

	size_t column() const noexcept;
};


}


#endif
