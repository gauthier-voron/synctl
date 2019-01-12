#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "synctl/io/AdapterInputStream.hxx"
#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/ui/Configuration.hxx"
#include "synctl/ui/ConfigurationException.hxx"
#include "synctl/DirectoryV1.hxx"
#include "synctl/Filter.hxx"
#include "synctl/FirstMatchFilter.hxx"
#include "synctl/GlobPattern.hxx"
#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/LoadContext.hxx"
#include "synctl/ReceiveContext.hxx"
#include "synctl/Reference.hxx"
#include "synctl/Repository.hxx"
#include "synctl/SendContext.hxx"
#include "synctl/StoreContext.hxx"
#include "synctl/Type.hxx"


using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::make_unique;
using std::move;
using std::string;
using std::vector;
using std::unique_ptr;
using synctl::AdapterInputStream;
using synctl::AdapterOutputStream;
using synctl::Configuration;
using synctl::ConfigurationException;
using synctl::DirectoryV1;
using synctl::Filter;
using synctl::FirstMatchFilter;
using synctl::GlobPattern;
using synctl::HashOutputStream;
using synctl::InputStream;
using synctl::LoadContext;
using synctl::Pattern;
using synctl::ReceiveContext;
using synctl::Reference;
using synctl::Repository;
using synctl::SendContext;
using synctl::StoreContext;
using synctl::Sender;
using synctl::Type;


// static int server_push()
// {
// 	Repository repository("sandbox");
// 	AdapterInputStream ais(std::cin);
// 	unique_ptr<TransientOutputStream> tos;
// 	vector<Reference> children;
// 	unique_ptr<Storer> storer;
// 	ObjectFactory factory;
// 	Reference reference;

// 	system("rm -rf sandbox");
// 	repository.initialize();

// 	while ((storer = factory.instance(&ais)) != nullptr) {
// 		tos = repository.newObject();
// 		reference = storer->store(&ais, tos.get(), &children);
// 		for (const Reference &ref : children)
// 			repository.takeReference(ref);
// 		tos->commit(reference);
// 		children.clear();
// 	}

// 	repository.newBranch("Orme")->newSnapshot(reference);

// 	return 0;
// }

int main_client_pull()
{
	AdapterInputStream ais = AdapterInputStream(cin);
	ReceiveContext ctx("recvbox", &ais);
	DirectoryV1 dir;
	Reference ref;
	Type type;

	system("rm -rf recvbox");

	ais.readall(&type, sizeof (type));
	dir.recv("/", &ctx);

	return 0;
}

int main_client_push()
{
	AdapterOutputStream aos = AdapterOutputStream(cout);
	SendContext ctx("/tmp", &aos, nullptr);
	DirectoryV1 dir;
	Reference ref;

	dir.send(&ref, "/", &ctx);

	return 0;
}

int main_server_pull()
{
	AdapterInputStream ais = AdapterInputStream(cin);
	Repository repository("sandbox");
	StoreContext ctx(&repository, &ais);
	DirectoryV1 dir;
	Reference ref;
	Type type;

	system("rm -rf sandbox");
	repository.initialize();

	ais.readall(&type, sizeof (type));
	dir.store(&ref, &ctx);

	repository.newBranch("Orme")->newSnapshot(ref);

	return 0;
}

int main_server_push(const string &hexref)
{
	unique_ptr<InputStream> ris;
	AdapterOutputStream aos = AdapterOutputStream(cout);
	Repository repository("sandbox");
	LoadContext ctx(&repository, &aos, nullptr);
	DirectoryV1 dir;
	Reference ref;
	Type type;

	ref = Reference::fromHex(hexref);
	ris = repository.readObject(ref);
	ris->readall(&type, sizeof (type));
	dir.load(ris.get(), "/", &ctx);

	return 0;
	
}

int main(int argc, const char **argv)
{
	Configuration config;

	config.load(argc, argv);
 	
	
	// if (argc < 2) {
	// 	cerr << "Indicate 'client' or 'server' as 1st arg" << endl;
	// 	return 1;
	// }

	// if (string(argv[1]) == "client-push") {
	// 	return main_client_push();
	// } else if (string(argv[1]) == "client-pull") {
	// 	return main_client_pull();
	// } else if (string(argv[1]) == "server-push") {
	// 	return main_server_push(string(argv[2]));
	// } else if (string(argv[1]) == "server-pull") {
	// 	return main_server_pull();
	// } else {
	// 	cerr << "Indicate either 'client' or 'server'" << endl;
	// 	return 1;
	// }

	return 0;
}
