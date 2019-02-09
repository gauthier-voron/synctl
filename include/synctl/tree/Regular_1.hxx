#ifndef _INCLUDE_SYNCTL_REGULAR_1_HXX_
#define _INCLUDE_SYNCTL_REGULAR_1_HXX_


#include <memory>

#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"


namespace synctl {


class Reference;


class Regular_1
{
	std::unique_ptr<OutputStream>   _destKeeper = nullptr;
	std::unique_ptr<InputStream>    _srcKeeper = nullptr;
	OutputStream                   *_dest = nullptr;
	InputStream                    *_src = nullptr;


	void _write(OutputStream *output) const;

	void _read(InputStream *input);


 public:
	Regular_1() = default;
	Regular_1(OutputStream *dest);
	Regular_1(std::unique_ptr<OutputStream> dest);
	Regular_1(InputStream *src);
	Regular_1(std::unique_ptr<InputStream> src);
	Regular_1(InputStream *src, OutputStream *dest);
	Regular_1(std::unique_ptr<InputStream> src,
		  std::unique_ptr<OutputStream> dest);
	Regular_1(Regular_1 &&other) = default;

	Regular_1 &operator=(Regular_1 &&other) = default;


	void setOutput(OutputStream *dest);
	void setOutput(std::unique_ptr<OutputStream> dest);

	void setInput(InputStream *src);
	void setInput(std::unique_ptr<InputStream> dest);


	void write(OutputStream *output, Reference *ref) const;

	void read(InputStream *input, Reference *ref);


	static Regular_1 make(const std::string &path);
	static Regular_1 makeTo(const std::string &path);
	static Regular_1 makeFrom(const std::string &path);
};


}


#endif
