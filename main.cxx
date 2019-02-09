#include "synctl/ui/Configuration.hxx"


using synctl::Configuration;


int main(int argc, const char **argv)
{
	Configuration config;

	config.load(argc, argv);

	return 0;
}
