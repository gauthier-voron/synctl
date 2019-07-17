#ifndef _INCLUDE_SYNCTL_EXPLORER_HXX_
#define _INCLUDE_SYNCTL_EXPLORER_HXX_


#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "synctl/plan/Opcode.hxx"
#include "synctl/repo/Snapshot.hxx"
#include "synctl/tree/Reference.hxx"


namespace synctl {


class Repository;


class Explorer
{
 private:
	struct Node
	{
		Reference                                     reference;
		opcode_t                                      opcode;
		uint16_t                                      mode;
		uint64_t                                      atime;
		uint64_t                                      mtime;
		std::string                                   user;
		std::string                                   group;
		std::map<std::string, std::string>            xattrs;
		bool                                          cloaded;
		std::map<std::string, std::shared_ptr<Node>>  children;

		Node() = default;
		Node(const Reference &reference, opcode_t opcode);
		Node(const Reference &reference, opcode_t opcode,
		     uint16_t mode, uint64_t atime, uint64_t mtime,
		     const std::string &user, const std::string &group,
		     const std::map<std::string, std::string> &xattrs);
		Node(const Node &other) = default;
	};


 public:
	class Entry
	{
		std::shared_ptr<Node>  _node;


	 public:
		Entry() = default;
		Entry(const std::shared_ptr<Node> &node);
		Entry(std::shared_ptr<Node> &&node);
		Entry(const Entry &other) = default;

		Entry &operator=(const Entry &entry) = default;

		bool operator==(const Entry &other) const;
		bool operator!=(const Entry &other) const;
		bool operator<(const Entry &other) const;

		uint16_t mode() const;
		uint64_t atime() const;
		uint64_t mtime() const;

		const std::string &user() const;
		const std::string &group() const;

		const Reference &reference() const;

		opcode_t opcode() const;

		const std::map<std::string, std::string> &xattrs() const;

		bool exist() const;
		bool isDirectory() const;

		Node *node() const;
	};


 private:
	struct Locator
	{
		Reference    root;
		std::string  path;

		Locator(const Reference &root, const std::string &path);
		Locator(const Locator &other) = default;

		bool operator<(const Locator &other) const;
	};


	const Repository          *_repository;
	std::map<Locator, Entry>   _entries;


	Entry _load(const Snapshot::Content &snap, const Locator &locator);

	void _loadDirectory_1(Node *node);

	void _loadChildren(Node *node);


 public:
	Explorer() = default;
	explicit Explorer(const Repository *repository);
	Explorer(Explorer &&other) = default;

	Explorer &operator=(Explorer &&other) = default;


	Entry find(const Snapshot::Content &snap, const std::string &path);

	std::map<std::string, Entry> list(const Snapshot::Content &snap,
					  const std::string &path);

	std::map<std::string, Entry> list(const Entry &entry);
};


}


#endif
