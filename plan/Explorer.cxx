// Copyright 2019 Gauthier Voron
//
// This file is part of Synctl
//
// Synctl is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Fundation, either version 3 of the License, or (at your option) any later
// version.
//
// Synctl is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANDABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// Synctl. If not, see <https://www.gnu.org/licenses/>.
//


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
		     uint16_t _mode, uint64_t _atime, uint64_t _mtime,
		     const string &_user, const string &_group,
		     const map<string, string> &_xattrs)
	: reference(_reference), opcode(_opcode), mode(_mode), atime(_atime)
	, mtime(_mtime), user(_user), group(_group), xattrs(_xattrs)
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
	if (_node->mode != other._node->mode)
		return false;
	if (_node->user != other._node->user)
		return false;
	if (_node->group != other._node->group)
		return false;
	if (_node->atime != other._node->atime)
		return false;
	if (_node->mtime != other._node->mtime)
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
	if (_node->mode < other._node->mode)
		return true;
	if (_node->user < other._node->user)
		return true;
	if (_node->group < other._node->group)
		return true;
	if (_node->atime < other._node->atime)
		return true;
	if (_node->mtime < other._node->mtime)
		return true;
	if (xattrs() < other.xattrs())
		return true;

	return false;
}

uint16_t Explorer::Entry::mode() const
{
	return _node->mode;
}

uint64_t Explorer::Entry::atime() const
{
	return _node->atime;
}

uint64_t Explorer::Entry::mtime() const
{
	return _node->mtime;
}

const string &Explorer::Entry::user() const
{
	return _node->user;
}

const string &Explorer::Entry::group() const
{
	return _node->group;
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
				      child.mode(), child.atime(),
				      child.mtime(), child.user(),
				      child.group(), child.xattrs());
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
