#include <synctl/plan/Protocol_1_0_0.hxx>

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/Channel.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/plan/Describe_1.hxx"
#include "synctl/plan/List_1.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/plan/ProtocolException.hxx"
#include "synctl/plan/ProtocolVersion.hxx"
#include "synctl/plan/ProtocolVersion_1_0_0.hxx"
#include "synctl/plan/Pull_1.hxx"
#include "synctl/plan/Push_1.hxx"
#include "synctl/plan/Receive_1.hxx"
#include "synctl/plan/Send_1.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/repo/Trunk.hxx"
#include "synctl/tree/FilterCodec.hxx"
#include "synctl/tree/Reference.hxx"


using std::move;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::Channel;
using synctl::InputStream;
using synctl::OutputStream;
using synctl::Protocol;
using synctl::Protocol_1_0_0;
using synctl::ProtocolException;
using synctl::ProtocolVersion;
using synctl::ProtocolVersion_1_0_0;
using synctl::Pull_1;
using synctl::Push_1;
using synctl::Receive_1;
using synctl::Reference;
using synctl::Repository;
using synctl::Send_1;
using synctl::Trunk;


Protocol_1_0_0::Protocol_1_0_0(Channel *channel)
	: _channel(channel)
{
}

const ProtocolVersion *Protocol_1_0_0::version() const noexcept
{
	return ProtocolVersion_1_0_0::get();
}


void Protocol_1_0_0::exit() const
{
	opcode_t op = OP_ACT_EXIT;

	_channel->outputStream()->writeInt(op);
}

void Protocol_1_0_0::list(const ListSettings &settings) const
{
	opcode_t op = OP_ACT_LIST;
	List_1 lister;
	string str;

	_channel->outputStream()->writeInt(op);
	_channel->outputStream()->writeStr(settings.trunkRegex);
	_channel->outputStream()->writeStr(settings.branchRegex);

	auto handler = [dest = settings.results]
		(const std::string &trunk,
		 const std::string &snapshot,
		 const struct timespec &date,
		 const std::vector<std::string> &branches) {

		dest->emplace_back();

		dest->back().trunk = trunk;
		dest->back().name = snapshot;
		dest->back().date = date;
		dest->back().branches = branches;
	};

	lister.list(_channel->inputStream(), handler);
}

void Protocol_1_0_0::_serveList(Repository *repository) const
{
	Describe_1 describer;

	describer.trunkRegex() = _channel->inputStream()->readStr();
	describer.branchRegex() = _channel->inputStream()->readStr();

	describer.describe(_channel->outputStream(), repository);
}

void Protocol_1_0_0::push(const PushSettings &settings) const
{
	Reference zeroref = Reference::zero();
	string snapshotName, *nameptr;
	opcode_t op = OP_ACT_PUSH;
	FilterCodec codec;
	Reference ref;
	Push_1 pusher;

	_channel->outputStream()->writeInt(op);
	_channel->outputStream()->writeStr(settings.trunkName);
	_channel->outputStream()->writeStr(settings.branchName);

	while (1) {
		_channel->inputStream()->readall(ref.data(), ref.size());
		if (ref == zeroref)
			break;
		pusher.addKnownReference(ref);
	}

	codec.encode(settings.filter, _channel->outputStream());

	pusher.setFilter(settings.filter);
	pusher.push(_channel->outputStream(), settings.localRoot);

	if (settings.snapshotName == nullptr)
		nameptr = &snapshotName;
	else
		nameptr = settings.snapshotName;

	*nameptr = _channel->inputStream()->readStr();
}

void Protocol_1_0_0::_servePush(Repository *repository) const
{
	Reference ref = Reference::zero();
	Snapshot::Content snapshot;
	unique_ptr<Filter> filter;
	Branch::Content branch;
	Receive_1 receiver;
	FilterCodec codec;
	string branchName;
	Snapshot *base;
	Trunk *trunk;

	branch.trunk = _channel->inputStream()->readStr();
	branchName = _channel->inputStream()->readStr();

	trunk = repository->trunk(branch.trunk);
	if (trunk == nullptr)
		trunk = repository->newTrunk(branch.trunk);

	repository->dumpReferences(_channel->outputStream());
	_channel->outputStream()->write(ref.data(), ref.size());

	filter = codec.decode(_channel->inputStream());
	base = trunk->lastSnapshot();

	if (base != nullptr)
		receiver.setBaseFilter(base->content(), filter.get());

	receiver.receive(_channel->inputStream(), repository, &snapshot);
	repository->takeReference(snapshot.tree);
	repository->takeReference(snapshot.links);

	trunk->newSnapshot(snapshot, &branch.snapshot);
	repository->setBranch(branchName, branch);

	_channel->outputStream()->writeStr(branch.snapshot);
}

void Protocol_1_0_0::pull(const PullSettings &settings) const
{
	opcode_t op = OP_ACT_PULL;
	FilterCodec codec;
	Pull_1 puller;

	_channel->outputStream()->writeInt(op);
	_channel->outputStream()->writeStr(settings.trunkName);
	_channel->outputStream()->writeStr(settings.snapshotName);

	codec.encode(settings.filter, _channel->outputStream());

	op = _channel->inputStream()->readInt<opcode_t>();

	switch (op) {
	case OP_RET_OK:
		break;
	default:
		throw ProtocolException();
	}

	puller.pull(_channel->inputStream(), settings.localRoot);
}

void Protocol_1_0_0::_servePull(Repository *repository) const
{
	string trunkName, snapshotName;
	const Snapshot *snapshot, *sn;
	unique_ptr<Filter> filter;
	const Trunk *trunk;
	FilterCodec codec;
	Send_1 sender;
	opcode_t op;

	_channel->inputStream()->readStr(&trunkName);
	_channel->inputStream()->readStr(&snapshotName);

	filter = codec.decode(_channel->inputStream());

	trunk = repository->trunk(trunkName);

	if (trunk == nullptr) {
		op = OP_RET_INVTRUNK;
		_channel->outputStream()->writeInt(op);
		return;
	}

	if (snapshotName.empty()) {
		snapshot = nullptr;
		for (const auto &pair : trunk->snapshots()) {
			sn = pair.second;

			if (snapshot == nullptr)
				snapshot = sn;
			else if (sn->content().date > snapshot->content().date)
				snapshot = sn;
		}
	} else {
		snapshot = trunk->snapshot(snapshotName);
	}

	if (snapshot == nullptr) {
		op = OP_RET_INVSNAPSHOT;
		_channel->outputStream()->writeInt(op);
		return;
	}

	op = OP_RET_OK;
	_channel->outputStream()->writeInt(op);

	sender.setFilter(filter.get());
	sender.send(_channel->outputStream(), repository, snapshot->content());
}

void Protocol_1_0_0::serve(Repository *repository) const
{
	opcode_t op;

	do {
		op = _channel->inputStream()->readInt<opcode_t>();
		switch (op) {
		case OP_ACT_EXIT:
			break;
		case OP_ACT_LIST:
			_serveList(repository);
			break;
		case OP_ACT_PUSH:
			_servePush(repository);
			break;
		case OP_ACT_PULL:
			_servePull(repository);
			break;
		}
	} while (op != OP_ACT_EXIT);
}
