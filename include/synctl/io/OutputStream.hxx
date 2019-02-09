#ifndef _INCLUDE_SYNCTL_OUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_OUTPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>
#include <string>


namespace synctl {


class OutputStream
{
 public:
	virtual ~OutputStream();

	virtual void write(uint8_t c) = 0;
	virtual void write(const uint8_t *src, size_t len);

	template<typename T> void write(const T *src, size_t len);

	virtual void writeStr(const std::string &str);

	virtual void close();
};


}


template<typename T>
void synctl::OutputStream::write(const T *src, size_t len)
{
	write(reinterpret_cast<const uint8_t *> (src), len);
}


#endif
