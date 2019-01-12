#ifndef _INCLUDE_SYNCTL_FILEOUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_FILEOUTPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>
#include <string>

#include "synctl/FdOutputStream.hxx"


namespace synctl {


class FileOutputStream : public FdOutputStream
{
 public:
	FileOutputStream();
	FileOutputStream(const std::string path);
	FileOutputStream(const FileOutputStream &other) = delete;
	FileOutputStream(FileOutputStream &&other);
	virtual ~FileOutputStream() = default;

	FileOutputStream &operator=(const FileOutputStream &other) = delete;
	FileOutputStream &operator=(FileOutputStream &&other);
};


}


#endif
