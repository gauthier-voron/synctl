#include "synctl/repo/Branch.hxx"

#include <string>

#include "synctl/io/FileInputStream.hxx"
#include "synctl/io/FileOutputStream.hxx"


using std::string;
using synctl::Branch;
using synctl::FileInputStream;
using synctl::FileOutputStream;


Branch::Branch(const string &path)
	: _path(path)
{
}

void Branch::store(const Content &content)
{
	FileOutputStream out = FileOutputStream(_path);

	out.writeStr(content.trunk);
	out.writeStr(content.snapshot);

	_loaded = true;
}

void Branch::load() const
{
	FileInputStream input = FileInputStream(_path);

	_content.trunk = input.readStr();
	_content.snapshot = input.readStr();

	_loaded = true;
}

const Branch::Content &Branch::content() const
{
	if (_loaded == false)
		load();

	return _content;
}

const string &Branch::path() const noexcept
{
	return _path;
}
