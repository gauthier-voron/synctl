#include "synctl/Snapshot.hxx"

#include <chrono>
#include <string>

#include "synctl/io/FileInputStream.hxx"
#include "synctl/io/FileOutputStream.hxx"
#include "synctl/io/IOException.hxx"
#include "synctl/Reference.hxx"


#define UINT64_STRING_MAXLEN      20
#define REFERENCE_STRING_MAXLEN   20


using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::system_clock;
using std::stoll;
using std::string;
using std::to_string;
using synctl::FileInputStream;
using synctl::FileOutputStream;
using synctl::IOException;
using synctl::Reference;
using synctl::Snapshot;


void Snapshot::_ensureLoaded() const
{
	if (_loaded == false)
		load();
}

Snapshot::Snapshot(const string &path)
	: _dir(path)
{
}

void Snapshot::initialize(const Reference &reference)
{
	Date now = duration_cast<nanoseconds>
		(system_clock::now().time_since_epoch()).count();
	initialize(now, reference);
}

void Snapshot::initialize(Snapshot::Date date, const Reference &reference)
{
	FileOutputStream out;
	string path, data;

	_dir.create();

	path = _dir.path() + "/date";
	data = to_string(date);
	out = FileOutputStream(path);
	out.write(data.data(), data.size());
	_date = date;

	path = _dir.path() + "/ref";
	data = reference.toHex();
	out = FileOutputStream(path);
	out.write(data.data(), data.size());
	_ref = reference;

	_loaded = true;
}

void Snapshot::load() const
{
	FileInputStream input;
	string path, data;
	size_t size;

	path = _dir.path() + "/date";
	input = FileInputStream(path);
	data.resize(UINT64_STRING_MAXLEN);
	size = input.read(data.data(), data.size());
	data.resize(size);
	_date = stoll(data);

	path = _dir.path() + "/ref";
	input = FileInputStream(path);
	data.resize(REFERENCE_STRING_MAXLEN);
	size = input.read(data.data(), data.size());
	data.resize(size);
	_ref = Reference::fromHex(data);

	_loaded = true;
}

Snapshot::Date Snapshot::date() const
{
	_ensureLoaded();
	return _date;
}

const Reference &Snapshot::ref() const
{
	_ensureLoaded();
	return _ref;
}
