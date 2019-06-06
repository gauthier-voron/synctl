#ifndef _INCLUDE_SYNCTL_LOGOUTPUTSTREAM_HXX_
#define _INCLUDE_SYNCTL_LOGOUTPUTSTREAM_HXX_


#include <cstdint>
#include <functional>
#include <string>

#include "synctl/io/OutputStream.hxx"


namespace synctl {


class LogOutputStream : public OutputStream
{
	using Logger = std::function<void(size_t)>;


	OutputStream  *_inner = nullptr;
	size_t         _count = 0;
	Logger         _logger = [](size_t) {};


 public:
	LogOutputStream() = default;
	LogOutputStream(OutputStream *inner);
	LogOutputStream(const LogOutputStream &other) = delete;
	LogOutputStream(LogOutputStream &&other);

	LogOutputStream &operator=(const LogOutputStream &other) = delete;
	LogOutputStream &operator=(LogOutputStream &&other);

	void setLogger(std::function<void(size_t did)> logger);

	size_t count() const;
	size_t reset(size_t count = 0);

	virtual void write(uint8_t c) override;
	virtual void write(const uint8_t *src, size_t len) override;
	virtual void write(const std::string &str) override;
	virtual void writeStr(const std::string &str) override;

	virtual void flush() override;

	using OutputStream::close;
};


}


#endif
