#include "synctl/tree/Symlink_1.hxx"

#include <unistd.h>

#include <string>

#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Reference.hxx"


#define SYMLINK_PATH_BUFFER_SIZE  128


using std::string;
using synctl::HashOutputStream;
using synctl::InputStream;
using synctl::OutputStream;
using synctl::Symlink_1;
using synctl::Reference;


Symlink_1::Symlink_1(const string &target)
	: _target(target)
{
}

void Symlink_1::_write(OutputStream *output) const
{
	output->writeStr(_target);
}

void Symlink_1::_read(InputStream *input)
{
	input->readStr(&_target);
}

void Symlink_1::setTarget(const string &target)
{
	_target = target;
}

string Symlink_1::getTarget() const
{
	return _target;
}

void Symlink_1::write(OutputStream *output, Reference *ref) const
{
	HashOutputStream hos;

	if (ref == nullptr) {
		_write(output);
	} else {
		hos = HashOutputStream(output);
		_write(&hos);
		hos.digest(ref);
	}
}

void Symlink_1::read(InputStream *input, Reference *ref)
{
	if (ref != nullptr)
		throw 0;  // not yet implemented
	_read(input);
}

Symlink_1 Symlink_1::make(const string &path)
{
	ssize_t s, l = SYMLINK_PATH_BUFFER_SIZE;
	string buffer;

	buffer.resize(l);

	while ((s = ::readlink(path.c_str(), buffer.data(), l)) == l) {
		l = l << 1;
		buffer.resize(l);
	}

	if (s < 0)
		throw 0;

	buffer.resize(s);
	return Symlink_1(buffer);
}
