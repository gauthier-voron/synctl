#include "synctl/plan/SnapshotCombiner.hxx"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "synctl/io/InputStream.hxx"
#include "synctl/io/Path.hxx"
#include "synctl/io/TransientOutputStream.hxx"
#include "synctl/plan/Explorer.hxx"
#include "synctl/repo/OverwriteException.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/repo/Snapshot.hxx"
#include "synctl/tree/Directory_1.hxx"
#include "synctl/tree/Filter.hxx"
#include "synctl/tree/Reference.hxx"


using std::map;
using std::make_unique;
using std::move;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::Directory_1;
using synctl::Explorer;
using synctl::Filter;
using synctl::InputStream;
using synctl::OverwriteException;
using synctl::Reference;
using synctl::Repository;
using synctl::Snapshot;
using synctl::SnapshotCombiner;
using synctl::TransientOutputStream;


Filter::Action SnapshotCombiner::_filterPath(const string &path) const
{
	Filter::Action ret;

	ret = _filter->apply(path);
	if (ret == Filter::Ignore)
		ret = Filter::Accept;

	return ret;
}

void SnapshotCombiner::_import(const string &path, Directory_1 *received,
			       const Explorer::Entry &base)
{
	string name;

	split(path, nullptr, &name);

	received->addChild(name, base.stat(), base.xattrs(),
			   base.opcode(), base.reference());

	_importedPaths.push_back(path);
}

void SnapshotCombiner::_store(const Directory_1 &object, Reference *reference)
{
	unique_ptr<TransientOutputStream> tos;
	Reference ref;

	for (const Directory_1::Entry &child : object.getChildren())
		_repository->takeReference(child.reference());

	tos = _repository->newObject();

	if (reference == nullptr)
		reference = &ref;
	object.write(tos.get(), reference);

	try {
		tos->commit(*reference);
	} catch (OverwriteException &e) {
		// May happen legally
	}
}

bool SnapshotCombiner::_combine(const string &path, Directory_1 *received,
				const Directory_1::Entry &recv,
				const Explorer::Entry &base)
{
	unique_ptr<InputStream> input;
	Reference reference;
	Directory_1 dir;
	bool modified;

	if (merged(path, &reference) == false) {
		input = _repository->readObject(recv.reference());
		dir.read(input.get());
		input.reset();

		modified = _merge(path, &dir, base);

		if (modified)
			_store(dir, &reference);
	} else {
		received->removeChild(recv.name());
		received->addChild(recv.name(), recv.stat(), recv.xattrs(),
				   recv.opcode(), reference);

		modified = true;
	}

	return modified;
}

bool SnapshotCombiner::_merge(const string &path, Directory_1 *received,
			      const Explorer::Entry &base)
{
	vector<Directory_1::Entry> recv = received->getChildren();
	map<string, Explorer::Entry> bchd = _explorer->list(base);
	bool hasRecv, modified = false;
	auto it = bchd.begin();
	string cpath = path;
	size_t clen, i;

	if (cpath[cpath.length() - 1] != '/')
		cpath.push_back('/');
	clen = cpath.length();

	i = 0;
	while (it != bchd.end()) {
		hasRecv = i < recv.size();
		if (hasRecv && (recv[i].name() < it->first)) {
			i++;
			continue;
		}

		cpath += it->first;

		if (!hasRecv || (it->first < recv[i].name())) {
			if (_filterPath(cpath) == Filter::Reject) {
				_import(cpath, received, it->second);
				modified = true;
			}

			it++;
			cpath.resize(clen);
			continue;
		}

		if (it->second.isDirectory() &&
		    (recv[i].opcode() == OP_TREE_DIRECTORY_1) &&
		    (recv[i].reference() != it->second.reference())) {
			modified |= _combine(cpath, received, recv[i],
					     it->second);
		}

		i++;
		it++;
		cpath.resize(clen);
	}

	return modified;
}

SnapshotCombiner::SnapshotCombiner(Repository *repository, Filter *filter,
				   const Snapshot::Content &base,
				   Explorer *explorer)
	: _repository(repository), _filter(filter), _base(base)
	, _explorer(explorer)
{
	if (_explorer == nullptr) {
		_privateExplorer = make_unique<Explorer>(repository);
		_explorer = _privateExplorer.get();
	}
}

SnapshotCombiner::SnapshotCombiner(SnapshotCombiner &&other)
	: _repository(other._repository), _filter(other._filter)
	, _base(other._base), _explorer(other._explorer)
	, _privateExplorer(move(other._privateExplorer))
	, _importedPaths(move(other._importedPaths))
	, _mergedPaths(move(other._mergedPaths))
{
	other._repository = nullptr;
	other._explorer = nullptr;
}

SnapshotCombiner &SnapshotCombiner::operator=(SnapshotCombiner &&other)
{
	_repository = other._repository;
	_filter = other._filter;
	_base = other._base;
	_explorer = other._explorer;
	_privateExplorer = move(other._privateExplorer);
	_importedPaths = move(other._importedPaths);
	_mergedPaths = move(other._mergedPaths);

	other._repository = nullptr;
	other._filter = nullptr;
	other._explorer = nullptr;

	return *this;
}

bool SnapshotCombiner::merge(const string &path, const Reference &reference,
			     Directory_1 *received)
{
	Explorer::Entry entry = _explorer->find(_base, path);

	if (entry.isDirectory() == false)
		return false;
	if (entry.reference() == reference)
		return false;

	return _merge(path, received, entry);
}

void SnapshotCombiner::assign(const string &path, const Reference &reference)
{
	_mergedPaths[path] = reference;
}

bool SnapshotCombiner::merged(const string &path, Reference *reference) const
{
	auto it = _mergedPaths.find(path);

	if (it == _mergedPaths.end())
		return false;

	if (reference != nullptr)
		*reference = it->second;

	return true;
}

const vector<string> &SnapshotCombiner::importedPaths() const
{
	return _importedPaths;
}
