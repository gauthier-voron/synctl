#include "synctl/ui/Configuration.hxx"

#include <vector>

#include "synctl/ui/Option.hxx"


using std::vector;
using synctl::Configuration;
using synctl::Option;


vector<Option *> Configuration::getOptions()
{
	vector<Option *> options;

	getOptions(&options);

	return options;
}
