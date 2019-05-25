#ifndef _INCLUDE_SYNCTL_OUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_OUTPUTSTREAM_HXX_


#include <endian.h>

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
	virtual void write(const std::string &str);

	template<typename T> void write(const T *src, size_t len);

	virtual void writeStr(const std::string &str);

	template<typename T> void writeInt(T num);

	virtual void close();
};


}


template<typename T>
void synctl::OutputStream::write(const T *src, size_t len)
{
	write(reinterpret_cast<const uint8_t *> (src), len);
}

template<>
inline void synctl::OutputStream::writeInt<uint8_t>(uint8_t num)
{
	write(&num, sizeof (num));
}

template<>
inline void synctl::OutputStream::writeInt<uint16_t>(uint16_t num)
{
	num = htole16(num);
	write(&num, sizeof (num));
}

template<>
inline void synctl::OutputStream::writeInt<uint32_t>(uint32_t num)
{
	num = htole32(num);
	write(&num, sizeof (num));
}

template<>
inline void synctl::OutputStream::writeInt<uint64_t>(uint64_t num)
{
	num = htole64(num);
	write(&num, sizeof (num));
}


#endif
