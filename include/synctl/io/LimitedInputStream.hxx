#ifndef _INCLUDE_SYNCTL_LIMITEDINPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_LIMITEDINPUTSTREAM_HXX_


#include <cstdint>

#include <memory>

#include "synctl/io/InputStream.hxx"


namespace synctl {


class LimitedInputStream : public InputStream
{
	std::unique_ptr<InputStream>   _attached;
	InputStream                   *_inner = nullptr;
	size_t                         _limit = 0;


 public:
	LimitedInputStream() = default;
	LimitedInputStream(InputStream *inner, size_t limit);
	LimitedInputStream(std::unique_ptr<InputStream> &&inner, size_t limit);
	LimitedInputStream(LimitedInputStream &&other) = default;
	virtual ~LimitedInputStream() = default;

	LimitedInputStream &operator=(LimitedInputStream &&other) = default;


	void reset(size_t limit);
	size_t remains() const;

	void attach(InputStream *inner);
	void attach(std::unique_ptr<InputStream> &&inner);


	virtual int16_t read();
	virtual size_t read(uint8_t *dest, size_t len);
	using InputStream::readall;

	virtual void close();
};


}


#endif
