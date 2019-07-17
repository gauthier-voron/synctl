#include "synctl/plan/Explorer.hxx"

#include <map>
#include <memory>
#include <string>

#include "synctl/io/InputStream.hxx"
#include "synctl/io/Path.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/repo/Snapshot.hxx"
#include "synctl/tree/Directory_1.hxx"
#include "synctl/tree/Reference.hxx"


using std::make_shared;
using std::map;
using std::move;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using synctl::Directory_1;
using synctl::Explorer;
using synctl::InputStream;
using synctl::Reference;
using synctl::Repository;
using synctl::Snapshot;


Explorer::Node::Node(const Reference &_reference, opcode_t _opcode)
	: reference(_reference), opcode(_opcode)
{
}

Explorer::Node::Node(const Reference &_reference, opcode_t _opcode,
		     const struct stat &_stat,
		     const map<string, string> &_xattrs)
	: reference(_reference), opcode(_opcode), stat(_stat), xattrs(_xattrs)
{
}

Explorer::Entry::Entry(const shared_ptr<Node> &node)
	: _node(node)
{
}

Explorer::Entry::Entry(shared_ptr<Node> &&node)
	: _node(node)
{
}

bool Explorer::Entry::operator==(const Entry &other) const
{
	if (opcode() != other.opcode())
		return false;

	if (_node == nullptr)
		return true;

	if (reference() != other.reference())
		return false;
	if (stat().st_mode != other.stat().st_mode)
		return false;
	// TODO: uid and gid might make no sense on server side
	if (stat().st_uid != other.stat().st_uid)
		return false;
	if (stat().st_gid != other.stat().st_gid)
		return false;
	if (stat().st_atim.tv_sec != other.stat().st_atim.tv_sec)
		return false;
	if (stat().st_atim.tv_nsec != other.stat().st_atim.tv_nsec)
		return false;
	if (stat().st_mtim.tv_sec != other.stat().st_mtim.tv_sec)
		return false;
	if (stat().st_mtim.tv_nsec != other.stat().st_mtim.tv_nsec)
		return false;
	if (xattrs() != other.xattrs())
		return false;

	return true;
}

bool Explorer::Entry::operator!=(const Entry &other) const
{
	return !(*this == other);
}

bool Explorer::Entry::operator<(const Entry &other) const
{
	if (opcode() < other.opcode())
		return true;

	if (_node == nullptr)
		return false;

	if (reference() < other.reference())
		return true;
	if (stat().st_mode < other.stat().st_mode)
		return true;
	// TODO: uid and gid might make no sense on server side
	if (stat().st_uid < other.stat().st_uid)
		return true;
	if (stat().st_gid < other.stat().st_gid)
		return true;
	if (stat().st_atim.tv_sec < other.stat().st_atim.tv_sec)
		return true;
	if (stat().st_atim.tv_nsec < other.stat().st_atim.tv_nsec)
		return true;
	if (stat().st_mtim.tv_sec < other.stat().st_mtim.tv_sec)
		return true;
	if (stat().st_mtim.tv_nsec < other.stat().st_mtim.tv_nsec)
		return true;
	if (xattrs() < other.xattrs())
		return true;

	return false;
}

const Reference &Explorer::Entry::reference() const
{
	return _node->reference;
}

opcode_t Explorer::Entry::opcode() const
{
	if (_node != nullptr)
		return _node->opcode;
	return OP_TREE_NONE;
}

const struct stat &Explorer::Entry::stat() const
{
	return _node->stat;
}

const map<string, string> &Explorer::Entry::xattrs() const
{
	return _node->xattrs;
}

bool Explorer::Entry::exist() const
{
	return (_node != nullptr);
}

bool Explorer::Entry::isDirectory() const
{
	return (opcode() == OP_TREE_DIRECTORY_1);
}

Explorer::Node *Explorer::Entry::node() const
{
	return _node.get();
}

Explorer::Locator::Locator(const Reference &_root, const std::string &_path)
	: root(_root), path(_path)
{
}

bool Explorer::Locator::operator<(const Locator &other) const
{
	if (root < other.root)
		return true;
	return (path < other.path);
}

Explorer::Entry Explorer::_load(const Snapshot::Content &snapshot,
				const Locator &locator)
{
	shared_ptr<Node> node;
	string parent, name;
	Entry entry;

	if (locator.path == "/") {
		node = make_shared<Node>(snapshot.tree, snapshot.opcode);
		entry = Entry(move(node));
		_entries[locator] = entry;
		return entry;
	}

	split(locator.path, &parent, &name);
	entry = find(snapshot, parent);

	for (const auto &child : list(entry)) {
		if (child.first != name)
			continue;
		return child.second;
	}

	return Entry();
}

void Explorer::_loadDirectory_1(Node *node)
{
	unique_ptr<InputStream> input;
	shared_ptr<Node> n;
	Directory_1 dir;

	input = _repository->readObject(node->reference);
	dir.read(input.get());
	input.reset();

	for (const Directory_1::Entry &child : dir.getChildren()) {
		n = make_shared<Node>(child.reference(), child.opcode(),
				      child.stat(), child.xattrs());
		node->children[child.name()] = move(n);
	}

	node->cloaded = true;
}

void Explorer::_loadChildren(Node *node)
{
	switch (node->opcode) {
	case OP_TREE_DIRECTORY_1:
		_loadDirectory_1(node);
		break;
	default:
		throw 0;
	}
}

Explorer::Explorer(const Repository *repository)
	: _repository(repository)
{
}

Explorer::Entry Explorer::find(const Snapshot::Content &snapshot,
			       const string &path)
{
	Locator locator = Locator(snapshot.tree, path);
	auto it = _entries.find(locator);

	if (it != _entries.end())
		return it->second;

	return _load(snapshot, locator);
}

map<string, Explorer::Entry> Explorer::list(const Snapshot::Content &snap,
					    const string &path)
{
	Entry entry = find(snap, path);

	if (entry.isDirectory() == false)
		return map<string, Explorer::Entry>();

	return list(entry);
}

map<string, Explorer::Entry> Explorer::list(const Entry &entry)
{
	map<string, Explorer::Entry> ret;
	Node *node;

	if (entry.isDirectory() == false)
		return ret;

	node = entry.node();

	if (node->cloaded == false)
		_loadChildren(node);

	for (const auto &child : node->children)
		ret[child.first] = Entry(child.second);

	return ret;
}
