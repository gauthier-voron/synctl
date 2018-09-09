#include "synctl/server/TransientOutputStream.hxx"

#include <cstdint> 
#include <cstdlib> 
#include <memory> 

#include "synctl/OutputStream.hxx"
#include "synctl/Reference.hxx"
#include "synctl/server/Repository.hxx"


using std::move;
using std::unique_ptr;
using synctl::OutputStream;
using synctl::Reference;
using synctl::server::Repository;
using synctl::server::TransientOutputStream;


TransientOutputStream::TransientOutputStream()
{
}

TransientOutputStream::TransientOutputStream(Repository *repository)
	: _repository(repository)
{
}

TransientOutputStream::TransientOutputStream(TransientOutputStream &&other)
	: _repository(other._repository), _buffer(move(other._buffer))
{
	other._repository = nullptr;
}

TransientOutputStream::~TransientOutputStream()
{
	close();
}

TransientOutputStream &
TransientOutputStream::operator=(TransientOutputStream &&other)
{
	_repository = other._repository;
	_buffer = move(other._buffer);
	other._repository = nullptr;

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
	unique_ptr<OutputStream> os = _repository->newObject(reference);

	os->write(_buffer.data(), _buffer.size());
	_buffer.clear();
}

void TransientOutputStream::close()
{
	_buffer.clear();
}
