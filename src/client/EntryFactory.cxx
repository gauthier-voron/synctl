#include "synctl/client/EntryFactory.hxx"

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <istream>
#include <memory>
#include <stdexcept>
#include <string>
#include <system_error>

#include "synctl/client/DirectoryV1.hxx"
#include "synctl/client/Receiver.hxx"
#include "synctl/client/RegularV1.hxx"
#include "synctl/client/Sender.hxx"
#include "synctl/client/SymlinkV1.hxx"


using std::istream;
using std::make_unique;
using std::runtime_error;
using std::string;
using std::system_error;
using std::unique_ptr;
using synctl::client::DirectoryV1;
using synctl::client::EntryFactory;
using synctl::client::Receiver;
using synctl::client::RegularV1;
using synctl::client::Sender;
using synctl::client::SymlinkV1;


EntryFactory::EntryFactory()
{
}

unique_ptr<Sender> EntryFactory::instance(const string &path) const
{
	struct stat st;
	int ret;

	ret = lstat(path.c_str(), &st);
	if (ret != 0)
		throw system_error(errno, std::generic_category());

	switch (st.st_mode & S_IFMT) {
	case S_IFDIR:  return make_unique<DirectoryV1>();
	case S_IFLNK:  return make_unique<SymlinkV1>();
	case S_IFREG:  return make_unique<RegularV1>();
	default:       return nullptr;
	}
}

unique_ptr<Receiver> EntryFactory::instance(istream &is, const string &p) const
{
	return nullptr;
}
