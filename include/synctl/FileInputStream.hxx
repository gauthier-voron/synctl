#ifndef _INCLUDE_SYNCTL_FILEINPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_FILEINPUTSTREAM_HXX_


#include <cstdint>
#include <string>

#include "synctl/InputStream.hxx"


namespace synctl {


class FileInputStream : public InputStream
{
	int  _fd;


 public:
	FileInputStream();
	FileInputStream(const std::string path);
	FileInputStream(const FileInputStream &other) = delete;
	FileInputStream(FileInputStream &&other);
	virtual ~FileInputStream();

	FileInputStream &operator=(const FileInputStream &other) = delete;
	FileInputStream &operator=(FileInputStream &&other);

	virtual int16_t read();
	virtual size_t read(uint8_t *dest, size_t len);
	using InputStream::read;

	virtual void readall(uint8_t *dest, size_t len);
	using InputStream::readall;

	virtual void close();
};	


}


#endif
