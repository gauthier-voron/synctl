#include "synctl/repo/Snapshot.hxx"

#include <chrono>
#include <string>

#include "synctl/io/FileInputStream.hxx"
#include "synctl/io/FileOutputStream.hxx"
#include "synctl/io/IOException.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Reference.hxx"


#define UINT64_STRING_MAXLEN      20
#define REFERENCE_STRING_MAXLEN   40


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


void Snapshot::_store() const
{
	FileOutputStream out;
	string path;

	path = _dir.path() + "/root";
	out = FileOutputStream(path);

	out.writeInt(_content.date);
	out.write(_content.tree.data(), _content.tree.size());

	_loaded = true;
}

void Snapshot::_ensureLoaded() const
{
	if (_loaded == false)
		load();
}

Snapshot::Snapshot(const string &path)
	: _dir(path)
{
}

void Snapshot::initialize(const Content &content)
{
	FileOutputStream out;
	string path, data;

	_content = content;

	if (_content.date == 0)
		_content.date = duration_cast<nanoseconds>
			(system_clock::now().time_since_epoch())
			.count();

	_dir.create();

	_store();
}

void Snapshot::load() const
{
	FileInputStream input;
	string path;

	path = _dir.path() + "/root";
	input = FileInputStream(path);

	_content.date = input.readInt<Date>();
	input.readall(_content.tree.data(), _content.tree.size());

	_loaded = true;
}

const Snapshot::Content &Snapshot::content() const
{
	_ensureLoaded();
	return _content;
}

const string &Snapshot::path() const noexcept
{
	return _dir.path();
}
