#ifndef _INCLUDE_SYNCTL_XATTRIBUTE_HXX_
#define _INCLUDE_SYNCTL_XATTRIBUTE_HXX_


#include <map>
#include <string>


namespace synctl {


std::string getXattribute(const std::string &path, const std::string &name);

void setXattribute(const std::string &path, const std::string &name,
		   const std::string &value);


void getXattributes(const std::string &path,
		    std::map<std::string, std::string> *dest);

std::map<std::string, std::string> getXattributes(const std::string &path);

void setXattributes(const std::string &path,
		    const std::map<std::string, std::string> &attrs);


}


#endif
