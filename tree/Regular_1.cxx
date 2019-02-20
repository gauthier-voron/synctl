#include "synctl/tree/Regular_1.hxx"

#include <memory>
#include <string>

#include "synctl/io/FileInputStream.hxx"
#include "synctl/io/FileOutputStream.hxx"
#include "synctl/io/HashOutputStream.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Reference.hxx"


#define TRANSFER_BUFFER_SIZE  16384


using std::make_unique;
using std::move;
using std::string;
using std::unique_ptr;
using synctl::FileInputStream;
using synctl::FileOutputStream;
using synctl::InputStream;
using synctl::OutputStream;
using synctl::Regular_1;


static void __transfer(OutputStream *out, InputStream *in)
{
	uint8_t buffer[TRANSFER_BUFFER_SIZE];
	size_t did;

	do {
		did = in->read(buffer, TRANSFER_BUFFER_SIZE);
		out->write(buffer, did);
	} while (did > 0);
}

void Regular_1::_write(OutputStream *output) const
{
	__transfer(output, _src);
}

void Regular_1::_read(InputStream *input)
{
	__transfer(_dest, input);
}

Regular_1::Regular_1(OutputStream *dest)
	: _dest(dest)
{
}

Regular_1::Regular_1(unique_ptr<OutputStream> dest)
	: _destKeeper(move(dest)), _dest(_destKeeper.get())
{
}

Regular_1::Regular_1(InputStream *src)
	: _src(src)
{
}

Regular_1::Regular_1(unique_ptr<InputStream> src)
	: _srcKeeper(move(src)), _src(_srcKeeper.get())
{
}

Regular_1::Regular_1(InputStream *src, OutputStream *dest)
	: _dest(dest), _src(src)
{
}

Regular_1::Regular_1(unique_ptr<InputStream> s, unique_ptr<OutputStream> d)
	: _destKeeper(move(d)), _srcKeeper(move(s)), _dest(_destKeeper.get()),
	  _src(_srcKeeper.get())
{
}

void Regular_1::setOutput(OutputStream *dest)
{
	_destKeeper = nullptr;
	_dest = dest;
}

void Regular_1::setOutput(unique_ptr<OutputStream> dest)
{
	_destKeeper = move(dest);
	_dest = _destKeeper.get();
}

void Regular_1::setInput(InputStream *src)
{
	_srcKeeper = nullptr;
	_src = src;
}

void Regular_1::setInput(unique_ptr<InputStream> src)
{
	_srcKeeper = move(src);
	_src = _srcKeeper.get();
}

void Regular_1::write(OutputStream *output, Reference *ref) const
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

void Regular_1::read(InputStream *input, Reference *ref)
{
	HashOutputStream hos;
	OutputStream *tmp;

	if (ref == nullptr) {
		_read(input);
	} else {
		// FIXME: Not safe
		hos = HashOutputStream(_dest);
		tmp = _dest;
		_dest = &hos;
		_read(input);
		_dest = tmp;
		hos.digest(ref);
	}
}

Regular_1 Regular_1::make(const string &path)
{
	unique_ptr<InputStream> fis = make_unique<FileInputStream>(path);
	unique_ptr<OutputStream> fos = make_unique<FileOutputStream>(path);

	return Regular_1(move(fis), move(fos));
}

Regular_1 Regular_1::makeTo(const string &path)
{
	unique_ptr<OutputStream> fos = make_unique<FileOutputStream>(path);

	return Regular_1(move(fos));
}

Regular_1 Regular_1::makeFrom(const string &path)
{
	unique_ptr<InputStream> fis = make_unique<FileInputStream>(path);

	return Regular_1(move(fis));
}
