#ifndef _INCLUDE_SYNCTL_NULLOUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_NULLOUTPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>

#include "synctl/io/OutputStream.hxx"


namespace synctl {


class NullOutputStream : public OutputStream
{
	size_t  _size = 0;


public:
	NullOutputStream() = default;
	virtual ~NullOutputStream() = default;

	virtual void write(uint8_t c) override;
	virtual void write(const uint8_t *src, size_t len) override;
	using OutputStream::write;

	size_t written() const;
	void reset();

	using OutputStream::flush;
	using OutputStream::close;
};


}


#endif
