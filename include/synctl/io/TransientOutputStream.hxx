#ifndef _INCLUDE_SYNCTL_TRANSIENTOUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_TRANSIENTOUTPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>
#include <string>

#include "synctl/io/OutputStream.hxx"


namespace synctl {


class ObjectStore;
class Reference;


class TransientOutputStream : public OutputStream
{
	ObjectStore  *_ostore = nullptr;
	std::string   _buffer;


 public:
	TransientOutputStream();
	TransientOutputStream(ObjectStore *ostore);
	TransientOutputStream(const TransientOutputStream &other) = delete;
	TransientOutputStream(TransientOutputStream &&other);
	virtual ~TransientOutputStream();

	TransientOutputStream &operator=(const TransientOutputStream &other)
	        = delete;
	TransientOutputStream &operator=(TransientOutputStream &&other);

	virtual void write(uint8_t c);
	virtual void write(const uint8_t *src, size_t len);
	using OutputStream::write;

	void commit(const Reference &reference);
	virtual void close();
};


}


#endif
