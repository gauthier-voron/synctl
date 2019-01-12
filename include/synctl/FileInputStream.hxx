#ifndef _INCLUDE_SYNCTL_FILEINPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_FILEINPUTSTREAM_HXX_


#include <cstdint>
#include <string>

#include "synctl/FdInputStream.hxx"


namespace synctl {


class FileInputStream : public FdInputStream
{
 public:
	FileInputStream();
	FileInputStream(const std::string path);
	FileInputStream(const FileInputStream &other) = delete;
	FileInputStream(FileInputStream &&other);
	virtual ~FileInputStream() = default;

	FileInputStream &operator=(const FileInputStream &other) = delete;
	FileInputStream &operator=(FileInputStream &&other);
};	


}


#endif
