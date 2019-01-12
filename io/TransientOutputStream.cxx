#include "synctl/io/TransientOutputStream.hxx"

#include <cstdint> 
#include <cstdlib> 
#include <memory> 

#include "synctl/io/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/ObjectStore.hxx"


using std::move;
using std::unique_ptr;
using synctl::OutputStream;
using synctl::Reference;
using synctl::ObjectStore;
using synctl::TransientOutputStream;


TransientOutputStream::TransientOutputStream()
{
}

TransientOutputStream::TransientOutputStream(ObjectStore *ostore)
	: _ostore(ostore)
{
}

TransientOutputStream::TransientOutputStream(TransientOutputStream &&other)
	: _ostore(other._ostore), _buffer(move(other._buffer))
{
	other._ostore = nullptr;
}

TransientOutputStream::~TransientOutputStream()
{
	close();
}

TransientOutputStream &
TransientOutputStream::operator=(TransientOutputStream &&other)
{
	_ostore = other._ostore;
	_buffer = move(other._buffer);
	other._ostore = nullptr;

	return *this;
}

void TransientOutputStream::write(uint8_t c)
{
	_buffer.push_back(static_cast<char> (c));
}

void TransientOutputStream::write(const uint8_t *src, size_t len)
{
	_buffer.append(reinterpret_cast<const char *> (src), len);
}

void TransientOutputStream::commit(const Reference &reference)
{
	unique_ptr<OutputStream> os = _ostore->newObject(reference);

	os->write(_buffer.data(), _buffer.size());
	_buffer.clear();
}

void TransientOutputStream::close()
{
	_buffer.clear();
}
