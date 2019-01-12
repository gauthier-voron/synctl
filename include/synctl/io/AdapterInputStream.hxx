#ifndef _INCLUDE_SYNCTL_ADAPTERINPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_ADAPTERINPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>
#include <istream>

#include "synctl/io/InputStream.hxx"


namespace synctl {


class AdapterInputStream : public InputStream
{
	std::istream  *_inner = nullptr;
	bool           _attached = false;


 public:
	AdapterInputStream();
	AdapterInputStream(std::istream &inner, bool attached = false);
	AdapterInputStream(const AdapterInputStream &other) = delete;
	AdapterInputStream(AdapterInputStream &&other);
	virtual ~AdapterInputStream() = default;

	AdapterInputStream &operator=(const AdapterInputStream &other)
	        = delete;
	AdapterInputStream &operator=(AdapterInputStream &&other);

	void assign(std::istream &inner);

	void attach();
	void attach(std::istream &inner);

	void detach();

	virtual int16_t read();
	virtual size_t read(uint8_t *src, size_t len);
	using InputStream::read;

	virtual void close();
};


}


#endif
