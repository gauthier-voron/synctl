#include "synctl/ui/Configurationzzz.hxx"

#include <vector>

#include "synctl/ui/Option.hxx"


using std::vector;
using synctl::Configurationzzz;
using synctl::Option;


vector<Option *> Configurationzzz::getOptions()
{
	vector<Option *> options;

	getOptions(&options);

	return options;
}
