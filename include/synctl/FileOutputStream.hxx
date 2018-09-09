#ifndef _INCLUDE_SYNCTL_FILEOUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_FILEOUTPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>
#include <string>

#include "synctl/OutputStream.hxx"


namespace synctl {


class FileOutputStream : public OutputStream
{
	int  _fd = -1;


 public:
	FileOutputStream();
	FileOutputStream(const std::string path);
	FileOutputStream(const FileOutputStream &other) = delete;
	FileOutputStream(FileOutputStream &&other);
	virtual ~FileOutputStream();

	FileOutputStream &operator=(const FileOutputStream &other) = delete;
	FileOutputStream &operator=(FileOutputStream &&other);

	virtual void write(uint8_t c);
	virtual void write(const uint8_t *src, size_t len);
	using OutputStream::write;

	virtual void close();
};


}


#endif
