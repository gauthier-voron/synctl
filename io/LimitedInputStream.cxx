#include "synctl/io/LimitedInputStream.hxx"

#include <cstdint>

#include <memory>

#include "synctl/io/EOFException.hxx"
#include "synctl/io/InputStream.hxx"


using std::move;
using std::unique_ptr;
using synctl::EOFException;
using synctl::LimitedInputStream;


LimitedInputStream::LimitedInputStream(InputStream *inner, size_t limit)
	: _inner(inner), _limit(limit)
{
}

LimitedInputStream::LimitedInputStream(unique_ptr<InputStream> &&inner,
				       size_t limit)
	: _attached(move(inner)), _inner(_attached.get()), _limit(limit)
{
}

void LimitedInputStream::reset(size_t limit)
{
	_limit = limit;
}

size_t LimitedInputStream::remains() const
{
	return _limit;
}

void LimitedInputStream::attach(InputStream *inner)
{
	_attached = nullptr;
	_inner = inner;
}

void LimitedInputStream::attach(unique_ptr<InputStream> &&inner)
{
	_attached = move(inner);
	_inner = _attached.get();
}

int16_t LimitedInputStream::read()
{
	int16_t ret;

	if (_limit == 0)
		return -1;

	ret = _inner->read();

	_limit -= 1;
	return ret;
}

size_t LimitedInputStream::read(uint8_t *dest, size_t len)
{
	size_t ret;

	if (_limit == 0)
		return 0;
	if (len > _limit)
		len = _limit;

	ret = _inner->read(dest, len);

	_limit -= ret;
	return ret;
}

void LimitedInputStream::close()
{
	_attached = nullptr;
	_inner = nullptr;
	_limit = 0;
}
