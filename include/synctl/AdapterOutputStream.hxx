#ifndef _INCLUDE_SYNCTL_ADAPTEROUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_ADAPTEROUTPUTSTREAM_HXX_


#include <cstdint>
#include <cstdlib>
#include <ostream>

#include "synctl/OutputStream.hxx"


namespace synctl {


class AdapterOutputStream : public OutputStream
{
	std::ostream  *_inner = nullptr;
	bool           _attached = false;


 public:
	AdapterOutputStream();
	AdapterOutputStream(std::ostream &inner, bool attached = false);
	AdapterOutputStream(const AdapterOutputStream &other) = delete;
	AdapterOutputStream(AdapterOutputStream &&other);
	virtual ~AdapterOutputStream() = default;

	AdapterOutputStream &operator=(const AdapterOutputStream &other)
	        = delete;
	AdapterOutputStream &operator=(AdapterOutputStream &&other);

	void assign(std::ostream &inner);

	void attach();
	void attach(std::ostream &inner);

	void detach();

	virtual void write(uint8_t c);
	virtual void write(const uint8_t *src, size_t len);
	using OutputStream::write;

	virtual void close();
};


}


#endif
