#include "synctl/Directory.hxx"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <algorithm>
#include <string>
#include <vector>

#include "synctl/FSException.hxx"


using std::move;
using std::sort;
using std::string;
using std::vector;
using synctl::Directory;
using synctl::FSException;


Directory::CreateException::CreateException()
{
}

Directory::CreateException::CreateException(const string &what)
	: FSException(what)
{
}


Directory::Directory()
{
}

Directory::Directory(const string &path)
	: _path(path)
{
}

bool Directory::exists() const
{
	struct stat st;
	int ret;

	ret = stat(_path.c_str(), &st);
	if (ret != 0)
		return false;

	if ((st.st_mode & S_IFMT) != S_IFDIR)
		return false;

	return true;
}

bool Directory::listable() const
{
	DIR *dh;

	if (exists() == false)
		return false;

	dh = opendir(_path.c_str());
	if (dh == nullptr)
		return false;

	closedir(dh);
	return true;
}

void Directory::create() const
{
	int ret;

	ret = mkdir(_path.c_str(), 0700);
	if (ret != 0)
		throw Directory::CreateException(_path);

	if (exists() == false)
		throw Directory::CreateException(_path);
}

Directory::ConstIterator::ConstIterator(void *handle)
	: _handle(handle)
{
}

Directory::ConstIterator::ConstIterator()
{
}

Directory::ConstIterator::ConstIterator(ConstIterator &&other)
	: _handle(other._handle), _current(move(other._current))
{
	other._handle = nullptr;
}

Directory::ConstIterator::~ConstIterator()
{
	if (_handle != nullptr)
		closedir((DIR *) _handle);
}

Directory::ConstIterator &Directory::ConstIterator::operator=
(ConstIterator &&other)
{
	_handle = other._handle;
	other._handle = nullptr;

	_current = move(other._current);

	return *this;
}

bool Directory::ConstIterator::operator==(const ConstIterator &other) const
{
	return (_handle == other._handle);
}

bool Directory::ConstIterator::operator!=(const ConstIterator &other) const
{
	return (_handle != other._handle);
}

Directory::ConstIterator &Directory::ConstIterator::operator++()
{
	DIR *dh = (DIR *) _handle;
	struct dirent *de;

	if (dh == nullptr)
		return *this;

	de = readdir(dh);
	
	if (de == nullptr) {
		closedir(dh);
		_handle = nullptr;
		_current.clear();
		return *this;
	}

	_current = string(de->d_name);
	return *this;
}

const string &Directory::ConstIterator::operator*() const
{
	return _current;
}

Directory::ConstIterator Directory::begin() const
{
	ConstIterator ret;
	DIR *dh;

	if (exists() == false)
		throw FSException(_path);

	dh = opendir(_path.c_str());
	if (dh == nullptr)
		throw IOException(_path);

	ret = ConstIterator((void *) dh);
	++ret;
	return ret;
}

Directory::ConstIterator Directory::end() const
{
	return ConstIterator();
}

vector<string> Directory::children() const
{
	vector<string> ret;

	for (const string &name : *this)
		ret.push_back(name);

	return ret;
}

vector<string> Directory::trueChildren() const
{
	vector<string> ret;

	for (const string &name : *this) {
		if (name == ".")
			continue;
		if (name == "..")
			continue;
		ret.push_back(name);
	}

	return ret;
}

vector<string> Directory::sortedChildren() const
{
	vector<string> ret = children();

	sort(ret.begin(), ret.end());

	return ret;
}

vector<string> Directory::sortedTrueChildren() const
{
	vector<string> ret = trueChildren();

	sort(ret.begin(), ret.end());

	return ret;
}

const string &Directory::path() const noexcept
{
	return _path;
}
