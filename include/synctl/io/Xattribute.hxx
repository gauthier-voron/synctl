#ifndef _INCLUDE_SYNCTL_XATTRIBUTE_HXX_
#define _INCLUDE_SYNCTL_XATTRIBUTE_HXX_


#include <map>
#include <string>


namespace synctl {


std::string getXattribute(const std::string &path, const std::string &name);


void getXattributes(const std::string &path,
		    std::map<std::string, std::string> *dest);

std::map<std::string, std::string> getXattributes(const std::string &path);


}


#endif
