#ifndef _INCLUDE_SYNCTL_SERVER_NEWREPOSITORYOBJECT_HXX_
#define _INCLUDE_SYNCTL_SERVER_NEWREPOSITORYOBJECT_HXX_


#include <ostream>
#include <string>


namespace synctl::server {


class NewRepositoryObject
{
 public:
	NewRepositoryObject(const Repository *rep, const std::string &path);
	~NewRepositoryObject();


	std::ostream stream();


	void commit(const synctl::Reference &ref);
};


}


#endif
