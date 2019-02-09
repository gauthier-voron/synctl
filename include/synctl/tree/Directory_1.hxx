#ifndef _INCLUDE_SYNCTL_DIRECTORY_1_HXX_
#define _INCLUDE_SYNCTL_DIRECTORY_1_HXX_


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <map>
#include <string>
#include <vector>

#include "synctl/tree/Reference.hxx"


namespace synctl {


class InputStream;
class OutputStream;


class Directory_1
{
        #pragma pack(1)
	struct EntryStat
	{
		uint8_t   dev;
		uint8_t   ino;
		uint16_t  mode;
		uint64_t  atime;
		uint64_t  mtime;
	};

	struct EntryInfo
	{
		EntryStat    stat;
		std::string  user;
		std::string  group;
		Reference    reference;

		EntryInfo() = default;
		EntryInfo(const struct stat &stat, const Reference &reference);
	};


 public:
	struct Entry
	{
		std::string  name;
		struct stat  stat;
		Reference    reference;

		Entry(const std::string &name, const struct stat &stat,
		      const Reference &reference);
		Entry(const std::string &name, const EntryInfo &einfo);
	};


 private:
	std::map<std::string, EntryInfo>  _children;


	void _writeInfo(OutputStream *output, const EntryInfo &einfo) const;
	void _write(OutputStream *output) const;

	bool _readCstr(InputStream *input, std::string *dest);
	void _readInfo(InputStream *input, EntryInfo *einfo);
	void _read(InputStream *input);


 public:
	Directory_1() = default;
	Directory_1(const Directory_1 &other) = default;

	Directory_1 &operator=(const Directory_1 &other) = default;


	void addChild(const std::string &name, const struct stat &statbuf,
		      const Reference &reference);

	void removeChild(const std::string &path);

	std::vector<Entry> getChildren() const;
	void getChildren(std::vector<Entry> *dest) const;

	size_t size() const;


	void write(OutputStream *output, Reference *ref = nullptr) const;

	void read(InputStream *input, Reference *ref = nullptr);
};


}


#endif
