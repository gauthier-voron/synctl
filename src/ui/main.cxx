#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "synctl/AdapterInputStream.hxx"
#include "synctl/AdapterOutputStream.hxx"
#include "synctl/HashOutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/client/EntryFactory.hxx"
#include "synctl/client/Sender.hxx"
#include "synctl/server/ObjectFactory.hxx"
#include "synctl/server/Repository.hxx"
#include "synctl/server/Storer.hxx"
#include "synctl/server/TransientOutputStream.hxx"


using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unique_ptr;
using synctl::AdapterInputStream;
using synctl::AdapterOutputStream;
using synctl::HashOutputStream;
using synctl::Reference;
using synctl::client::EntryFactory;
using synctl::client::Sender;
using synctl::server::ObjectFactory;
using synctl::server::Repository;
using synctl::server::Storer;
using synctl::server::TransientOutputStream;


static int server()
{
	Repository repository("sandbox");
	AdapterInputStream ais(std::cin);
	unique_ptr<TransientOutputStream> tos;
	vector<Reference> children;
	unique_ptr<Storer> storer;
	ObjectFactory factory;
	Reference reference;

	system("rm -rf sandbox");
	repository.initialize();

	while ((storer = factory.instance(&ais)) != nullptr) {
		tos = repository.newObject();
		reference = storer->store(&ais, tos.get(), &children);
		for (const Reference &ref : children)
			repository.takeReference(ref);
		tos->commit(reference);
		children.clear();
	}

	return 0;
}

int main(int argc, const char **argv)
{
	if (argc < 2) {
		cerr << "Indicate 'client' or 'server' as 1st arg" << endl;
		return 1;
	}

	if (string(argv[1]) == "client") {
		AdapterOutputStream aos(cout);
		EntryFactory efacto;
		unique_ptr<Sender> sender = efacto.instance("test");
		uint16_t zero = 0;
		Reference ref;

		sender->send(&aos, &ref);
		aos.write(&zero, sizeof (zero));

		return 0;
	} else if (string(argv[1]) == "server") {
		return server();
	} else {
		cerr << "Indicate either 'client' or 'server'" << endl;
		return 1;
	}


	// HashOutputStream hos;
	// Reference ref;

	// hos.write("coucou", 6);
	// hos.digest(&ref);
	// hos.close();

	// cout << ref.toHex() << endl;

	// Repository repo = Repository('repository');
	// LocalServer serv = LocalServer(&repo);
	// Hierarchy tree = Hierarchy("Orme", '/');
	// LocalConnection connect = LocalConnection(&serv);

	// connect.push(&tree);
}
