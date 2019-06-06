#ifndef _INCLUDE_SYNCTL_FDOUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_FDOUTPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>
#include <string>

#include "synctl/io/OutputStream.hxx"


namespace synctl {


class FdOutputStream : public OutputStream
{
	int  _fd = -1;


 public:
	FdOutputStream();
	FdOutputStream(int fd);
	FdOutputStream(const FdOutputStream &other) = delete;
	FdOutputStream(FdOutputStream &&other);
	virtual ~FdOutputStream();

	FdOutputStream &operator=(const FdOutputStream &other) = delete;
	FdOutputStream &operator=(FdOutputStream &&other);

	virtual void write(uint8_t c) override;
	virtual void write(const uint8_t *src, size_t len) override;
	using OutputStream::write;

	using OutputStream::flush;

	virtual void close() override;
};


}


#endif
