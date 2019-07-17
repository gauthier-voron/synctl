#ifndef _INCLUDE_SYNCTL_DIRECTORY_1_HXX_
#define _INCLUDE_SYNCTL_DIRECTORY_1_HXX_


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "synctl/plan/Opcode.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class InputStream;
class OutputStream;


class Directory_1
{
        #pragma pack(1)
	struct EntryStat
	{
		uint16_t  mode;
		uint64_t  atime;
		uint64_t  mtime;
	};

	struct EntryXattr
	{
		std::string  name;
		std::string  value;

		EntryXattr(const std::string &name, const std::string &value);
	};

	struct EntryInfo
	{
		EntryStat                stat;
		std::string              user;
		std::string              group;
		std::vector<EntryXattr>  xattrs;
		opcode_t                 opcode;
		Reference                reference;

		EntryInfo() = default;
		EntryInfo(const struct stat &stat,
			  const std::map<std::string, std::string> &xattrs,
			  opcode_t opcode, const Reference &reference);
	};


 public:
	class Entry
	{
		std::string                                  _name;
		std::shared_ptr<EntryInfo>                   _einfo;

		mutable struct stat                          _stat;
		mutable std::map<std::string, std::string>   _xattrs;


	 public:
		Entry();
		Entry(const std::string &name,
		      std::shared_ptr<EntryInfo> &einfo);

		Entry(const std::string &name,
		      std::shared_ptr<EntryInfo> &einfo,
		      const struct stat &stat,
		      const std::map<std::string, std::string> &xattrs);
		Entry(const Entry &other) = default;
		Entry(Entry &&other) = default;

		Entry &operator=(const Entry &other) = default;
		Entry &operator=(Entry &&other) = default;

		const std::string &name() const;

		const struct stat &stat() const;

		const std::map<std::string, std::string> &xattrs() const;

		opcode_t opcode() const;

		const Reference &reference() const;
	};


 private:
	std::map<std::string, std::shared_ptr<EntryInfo>>  _children;


	void _writeXattr(OutputStream *output, const EntryInfo &einfo) const;
	void _writeInfo(OutputStream *output, const EntryInfo &einfo) const;
	void _write(OutputStream *output) const;

	void _readXattr(InputStream *input, EntryInfo *einfo);
	void _readInfo(InputStream *input, EntryInfo *einfo);
	void _read(InputStream *input);


 public:
	Directory_1() = default;
	Directory_1(const Directory_1 &other) = default;

	Directory_1 &operator=(const Directory_1 &other) = default;


	void addChild(const std::string &name, const struct stat &statbuf,
		      const std::map<std::string, std::string> &xattrs,
		      opcode_t opcode, const Reference &reference);

	void removeChild(const std::string &path);

	std::vector<Entry> getChildren() const;
	void getChildren(std::vector<Entry> *dest) const;

	size_t size() const;


	void write(OutputStream *output, Reference *ref = nullptr) const;

	void read(InputStream *input, Reference *ref = nullptr);
};


}


#endif
