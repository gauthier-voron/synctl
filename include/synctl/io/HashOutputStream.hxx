#ifndef _INCLUDE_SYNCTL_HASHOUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_HASHOUTPUTSTREAM_HXX_


#include <openssl/sha.h>

#include <cstdint>
#include <cstdlib>

#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class HashOutputStream : public OutputStream
{
	SHA_CTX        _ctx;
	OutputStream  *_inner = nullptr;
	bool           _attached = false;


 public:
	HashOutputStream();
	HashOutputStream(OutputStream *inner, bool attached = false);
	HashOutputStream(const HashOutputStream &other) = delete;
	HashOutputStream(HashOutputStream &&other);
	virtual ~HashOutputStream();

	HashOutputStream &operator=(const HashOutputStream &other) = delete;
	HashOutputStream &operator=(HashOutputStream &&other);

	void assign(OutputStream *inner);

	void attach();
	void attach(OutputStream *inner);

	void detach();

	void digest(Reference *dest);
	Reference digest();

	virtual void write(uint8_t c);
	virtual void write(const uint8_t *src, size_t len);
	using OutputStream::write;

	virtual void flush() override;

	virtual void close();
};


}


#endif
