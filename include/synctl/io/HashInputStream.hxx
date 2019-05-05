#ifndef _INCLUDE_SYNCTL_HASHINPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_HASHINPUTSTREAM_HXX_


#include <openssl/sha.h>

#include <cstdint>
#include <cstdlib>

#include "synctl/io/InputStream.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class HashInputStream : public InputStream
{
	SHA_CTX       _ctx;
	InputStream  *_inner = nullptr;
	bool          _attached = false;


 public:
	HashInputStream();
	HashInputStream(InputStream *inner, bool attached = false);
	HashInputStream(const HashInputStream &other) = delete;
	HashInputStream(HashInputStream &&other);
	virtual ~HashInputStream();

	HashInputStream &operator=(const HashInputStream &other) = delete;
	HashInputStream &operator=(HashInputStream &&other);

	void assign(InputStream *inner);

	void attach();
	void attach(InputStream *inner);

	void detach();

	void digest(Reference *dest);
	Reference digest();

	virtual int16_t read();
	virtual size_t read(uint8_t *dest, size_t len);
	using InputStream::readall;

	virtual void close();
};


}


#endif
