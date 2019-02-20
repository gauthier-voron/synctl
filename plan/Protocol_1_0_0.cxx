#include <synctl/plan/Protocol_1_0_0.hxx>

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/Channel.hxx"
#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/plan/Opcode.hxx"
#include "synctl/plan/Protocol.hxx"
#include "synctl/plan/ProtocolException.hxx"
#include "synctl/plan/ProtocolVersion.hxx"
#include "synctl/plan/ProtocolVersion_1_0_0.hxx"
#include "synctl/plan/Pull_1.hxx"
#include "synctl/plan/Push_1.hxx"
#include "synctl/plan/Receive_1.hxx"
#include "synctl/plan/Send_1.hxx"
#include "synctl/repo/Branch.hxx"
#include "synctl/repo/Repository.hxx"
#include "synctl/tree/Reference.hxx"


using std::move;
using std::string;
using std::unique_ptr;
using std::vector;
using synctl::Branch;
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

	_channel->outputStream()->write(&op, sizeof (op));
}

void Protocol_1_0_0::push(const PushSettings &settings) const
{
	Reference zeroref = Reference::zero();
	opcode_t op = OP_ACT_PUSH;
	Reference ref, *rptr;
	Push_1 pusher;

	_channel->outputStream()->write(&op, sizeof (op));
	_channel->outputStream()->writeStr(settings.branchName);

	while (1) {
		_channel->inputStream()->readall(ref.data(), ref.size());
		if (ref == zeroref)
			break;
		pusher.addKnownReference(ref);
	}

	pusher.setFilter(settings.filter);
	pusher.push(_channel->outputStream(), settings.localRoot);

	if (settings.snapshotReference == nullptr)
		rptr = &ref;
	else
		rptr = settings.snapshotReference;

	_channel->inputStream()->read(rptr->data(), rptr->size());
}

void Protocol_1_0_0::_servePush(Repository *repository) const
{
	Reference ref = Reference::zero();
	Receive_1 receiver;
	Snapshot *snapshot;
	string branchName;
	Branch *branch;

	_channel->inputStream()->readStr(&branchName);

	branch = repository->branch(branchName);
	if (branch == nullptr)
		branch = repository->newBranch(branchName);

	repository->dumpReferences(_channel->outputStream());
	_channel->outputStream()->write(ref.data(), ref.size());

	receiver.receive(_channel->inputStream(), repository, &ref);
	repository->takeReference(ref);

	snapshot = branch->newSnapshot(ref);

	_channel->outputStream()->write(ref.data(), ref.size());
}

void Protocol_1_0_0::pull(const PullSettings &settings) const
{
	opcode_t op = OP_ACT_PULL;
	Pull_1 puller;

	_channel->outputStream()->write(&op, sizeof (op));
	_channel->outputStream()->writeStr(settings.branchName);
	_channel->outputStream()->writeStr(settings.snapshotName);

	_channel->inputStream()->read(&op, sizeof (op));

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
	string branchName, snapshotName;
	const Snapshot *snapshot;
	const Branch *branch;
	Send_1 sender;
	opcode_t op;

	_channel->inputStream()->readStr(&branchName);
	_channel->inputStream()->readStr(&snapshotName);

	branch = repository->branch(branchName);

	if (branch == nullptr) {
		op = OP_RET_INVBRANCH;
		_channel->outputStream()->write(&op, sizeof (op));
		return;
	}

	if (snapshotName.empty()) {
		snapshot = nullptr;
		for (const Snapshot *sn : branch->snapshots()) {
			if (snapshot == nullptr)
				snapshot = sn;
			else if (sn->date() > snapshot->date())
				snapshot = sn;
		}
	} else {
		snapshot = branch->snapshot(snapshotName);
	}

	if (snapshot == nullptr) {
		op = OP_RET_INVSNAPSHOT;
		_channel->outputStream()->write(&op, sizeof (op));
		return;
	}

	op = OP_RET_OK;
	_channel->outputStream()->write(&op, sizeof (op));

	sender.send(_channel->outputStream(), repository, snapshot->ref());
}

void Protocol_1_0_0::serve(Repository *repository) const
{
	opcode_t op;

	do {
		_channel->inputStream()->readall(&op, sizeof (op));
		switch (op) {
		case OP_ACT_EXIT:
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
