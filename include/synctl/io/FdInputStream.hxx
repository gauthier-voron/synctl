#ifndef _INCLUDE_SYNCTL_FDINPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_FDINPUTSTREAM_HXX_


#include <cstdint>

#include "synctl/io/InputStream.hxx"


namespace synctl {


class FdInputStream : public InputStream
{
	int  _fd = -1;

	
 public:
	FdInputStream();
	FdInputStream(int fd);
	FdInputStream(const FdInputStream &other) = delete;
	FdInputStream(FdInputStream &&other);
	virtual ~FdInputStream();

	FdInputStream &operator=(const FdInputStream &other) = delete;
	FdInputStream &operator=(FdInputStream &&other);

	virtual int16_t read();
	virtual size_t read(uint8_t *dest, size_t len);
	using InputStream::read;

	virtual void readall(uint8_t *dest, size_t len);
	using InputStream::readall;

	virtual void close();
};


}


#endif
