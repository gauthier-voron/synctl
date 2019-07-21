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


#include "synctl/plan/LinkBuilder.hxx"

#include <map>
#include <memory>
#include <set>
#include <string>

#include "synctl/io/InputStream.hxx"
#include "synctl/io/Path.hxx"
#include "synctl/io/TransientOutputStream.hxx"
#include "synctl/plan/Explorer.hxx"
#include "synctl/repo/OverwriteException.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/repo/Snapshot.hxx"
#include "synctl/tree/Link_1.hxx"
#include "synctl/tree/Linktable_1.hxx"


using std::make_unique;
using std::map;
using std::move;
using std::set;
using std::string;
using std::unique_ptr;
using synctl::Explorer;
using synctl::InputStream;
using synctl::Link_1;
using synctl::Linktable_1;
using synctl::LinkBuilder;
using synctl::OverwriteException;
using synctl::Repository;
using synctl::Snapshot;
using synctl::TransientOutputStream;


void LinkBuilder::MergeLeaves::clear()
{
	merged.clear();
	received.clear();
	recventry = nullptr;
}

void LinkBuilder::_loadBaseLink(const Snapshot::Content &base,
				const Link_1 &link)
{
	Link *ptr;

	_bstorage.emplace_back(make_unique<Link>());
	ptr = _bstorage.back().get();

	for (string path : link.getLocations()) {
		ptr->insert(path);

		if (ptr->entry.exist() == false)
			ptr->entry = _explorer->find(base, path);

		do {
			_baseprefix[path].insert(ptr);
			path = prefix(path);
		} while (path != "/");
	}
}

void LinkBuilder::_loadBaseTable(const Snapshot::Content &base,
				 const Linktable_1 &table)
{
	unique_ptr<InputStream> input;
	Link_1 link;

	for (const Reference &reference : table.getLinks()) {
		input = _repository->readObject(reference);
		link.read(input.get());
		input.reset();

		_loadBaseLink(base, link);
		link.clear();
	}
}

void LinkBuilder::_mergeLeaves(const MergeLeaves &leaves,
			       const Snapshot::Content &snapshot)
{
	unique_ptr<Link> single = make_unique<Link>();
	Explorer::Entry rentry;
	bool hasLink = false;
	Link *recvlink, *ptr;

	for (const string &path : leaves.received) {
		rentry = _explorer->find(snapshot, path);
		if (rentry != *leaves.recventry)
			continue;

		auto it = _received.find(path);

		if (it == _received.end()) {
			single->insert(path);
			ptr = single.get();
		} else {
			ptr = it->second;
		}

		if ((hasLink && (recvlink != ptr)) || (single->size() > 1)) {
			recvlink = nullptr;
			break;
		}

		recvlink = ptr;
		hasLink = true;
	}

	if (recvlink == single.get()) {
		_rstorage.push_back(move(single));
		recvlink = _rstorage.back().get();
		_received[*recvlink->begin()] = recvlink;
	}

	if (recvlink != nullptr) {
		for (const string &path : leaves.merged) {
			recvlink->insert(path);
			_received[path] = recvlink;
		}
	}
}

void LinkBuilder::_mergeNode(const string &path,
			     const Snapshot::Content &snapshot)
{
	auto it = _baseprefix.find(path);
	MergeLeaves leaves;

	if (it == _baseprefix.end())
		return;

	for (const Link *baselink : it->second) {
		leaves.clear();

		leaves.recventry = &baselink->entry;
		for (const string &leaf : (*baselink))
			if (leaf.find(path) == 0)
				leaves.merged.insert(leaf);
			else
				leaves.received.insert(leaf);

		_mergeLeaves(leaves, snapshot);
	}
}

void LinkBuilder::_mergeNodes(const Snapshot::Content &snapshot)
{
	for (const string &path : _merged)
		_mergeNode(path, snapshot);
}

void LinkBuilder::_buildLink(Link *link, Reference *dest)
{
	unique_ptr<TransientOutputStream> tos;
	Reference reference;
	Link_1 lk;

	for (const string &path : *link)
		lk.addLocation(path);

	tos = _repository->newObject();
	lk.write(tos.get(), dest);

	try {
		tos->commit(*dest);
	} catch (OverwriteException &e) {
		// may legally already exist
	}
}

LinkBuilder::LinkBuilder(Repository *repository, Explorer *explorer)
	: _repository(repository), _explorer(explorer)
{
	if (_explorer == nullptr) {
		_exprivate = make_unique<Explorer>(repository);
		_explorer = _exprivate.get();
	}
}

void LinkBuilder::bindReceived(const string &path0, const string &path1)
{
	auto it = _received.find(path0);

	if (it != _received.end()) {
		it->second->insert(path1);
		_received[path1] = it->second;
		return;
	}

	it = _received.find(path1);

	if (it != _received.end()) {
		it->second->insert(path0);
		_received[path0] = it->second;
		return;
	}

	_rstorage.emplace_back(make_unique<Link>());
	_rstorage.back()->insert(path0);
	_rstorage.back()->insert(path1);
	_received[path0] = _rstorage.back().get();
	_received[path1] = _rstorage.back().get();
}

void LinkBuilder::mergePath(const string &path)
{
	_merged.insert(path);
}

void LinkBuilder::loadBase(const Snapshot::Content &base)
{
	unique_ptr<InputStream> input;
	Linktable_1 table;

	input = _repository->readObject(base.links);
	table.read(input.get());
	input.reset();

	_loadBaseTable(base, table);
}

void LinkBuilder::buildLinktable(Snapshot::Content *dest)
{
	unique_ptr<TransientOutputStream> tos;
	Reference reference;
	Linktable_1 table;

	_mergeNodes(*dest);

	for (const LinkPtr &link : _rstorage) {
		_buildLink(link.get(), &reference);
		table.addLink(reference);
	}

	tos = _repository->newObject();
	table.write(tos.get(), &dest->links);

	try {
		tos->commit(dest->links);
	} catch (OverwriteException &e) {
		// may legally already exist
	}
}
