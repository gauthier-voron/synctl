#ifndef _INCLUDE_SYNTL_CLIENT_ENTRYFACTORY_HXX_
#define _INCLUDE_SYNTL_CLIENT_ENTRYFACTORY_HXX_


#include <istream>
#include <memory>
#include <string>

#include "synctl/Filter.hxx"


namespace synctl::client {


class Sender;
class Receiver;


class EntryFactory
{
 public:
	EntryFactory();


	std::unique_ptr<Sender> instance(const std::string &path) const;

	std::unique_ptr<Receiver> instance(std::istream &is,
					   const std::string &p) const;
};


}


#endif
