#ifndef _INCLUDE_SYNCTL_SERVER_REPOSITORYOBJECT_HXX_
#define _INCLUDE_SYNCTL_SERVER_REPOSITORYOBJECT_HXX_


#include <cstdint>
#include <istream>
#include <string>


namespace synctl::server {


class RepositoryObject
{
 public:
	RepositoryObject(const std::string &path);


	uint32_t getRefcount() const;
	void setRefcount(uint32_t refcount);


	std::istream open();
};


}


#endif
