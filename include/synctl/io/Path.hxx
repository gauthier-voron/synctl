#ifndef _INCLUDE_SYNCTL_PATH_HXX_
#define _INCLUDE_SYNCTL_PATH_HXX_


#include <string>

#include "synctl/io/FileOutputStream.hxx"


namespace synctl {


std::string randomHexPath(const std::string &root, size_t len);

FileOutputStream randomHexOutput(std::string *path, size_t len);

void split(const std::string &path, std::string *prefix, std::string *suffix);

std::string prefix(const std::string &path);


}


#endif
