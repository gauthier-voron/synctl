#include "synctl/io/TransientOutputStream.hxx"

#include <unistd.h>

#include <cstdint> 
#include <cstdlib> 
#include <memory> 

#include "synctl/io/FileOutputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Reference.hxx"
#include "synctl/repo/ObjectStore.hxx"


using std::move;
using std::string;
using std::unique_ptr;
using synctl::OutputStream;
using synctl::Reference;
using synctl::ObjectStore;
using synctl::TransientOutputStream;


TransientOutputStream::TransientOutputStream()
{
}

TransientOutputStream::TransientOutputStream(ObjectStore *ostore,
					     const string &path,
					     FileOutputStream &&output)
	: _ostore(ostore), _path(path), _output(move(output))
{
}

TransientOutputStream::TransientOutputStream(TransientOutputStream &&other)
	: _ostore(other._ostore), _path(move(other._path)),
	  _output(move(other._output))
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
	_path = move(other._path);
	_output = move(other._output);
	other._ostore = nullptr;

	return *this;
}

void TransientOutputStream::write(uint8_t c)
{
	_output.write(c);
}

void TransientOutputStream::write(const uint8_t *src, size_t len)
{
	_output.write(src, len);
}

void TransientOutputStream::commit(const Reference &reference)
{
	_output.close();
	_ostore->putObject(_path, reference);
	_path.clear();
}

void TransientOutputStream::flush()
{
	_output.flush();
}

void TransientOutputStream::close()
{
	if (_path.empty())
		return;

	_output.close();
	::unlink(_path.c_str());
	_path.clear();
}
