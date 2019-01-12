#ifndef _INCLUDE_SYNCTL_INPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_INPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>


namespace synctl {


class InputStream
{
 public:
	virtual ~InputStream();

	virtual int16_t read() = 0;
	virtual size_t read(uint8_t *dest, size_t len);
	virtual void readall(uint8_t *dest, size_t len);

	template<typename T> size_t read(T *dest, size_t len);
	template<typename T> void readall(T *dest, size_t len);

	virtual void close();
};


}


template<typename T>
size_t synctl::InputStream::read(T *dest, size_t len)
{
	return read(reinterpret_cast<uint8_t *> (dest), len);
}

template<typename T>
void synctl::InputStream::readall(T *dest, size_t len)
{
	readall(reinterpret_cast<uint8_t *> (dest), len);
}


#endif
