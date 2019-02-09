#include "synctl/plan/Protocol.hxx"

#include <memory>
#include <string>
#include <vector>

#include "synctl/io/InputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/io/Channel.hxx"
#include "synctl/plan/ProtocolException.hxx"
#include "synctl/plan/ProtocolVersion.hxx"
#include "synctl/plan/ProtocolVersion_0_0_0.hxx"
#include "synctl/plan/ProtocolVersion_1_0_0.hxx"


using std::string;
using std::unique_ptr;
using std::vector;
using synctl::Channel;
using synctl::InputStream;
using synctl::OutputStream;
using synctl::Protocol;
using synctl::ProtocolVersion;
using synctl::ProtocolVersion_0_0_0;
using synctl::ProtocolVersion_1_0_0;
using synctl::ProtocolException;


// Available protocol, sorted from the preferred ones to the deprecated ones.
// Always finishes with version 0.0.0
static const ProtocolVersion *__versions[] = {
	ProtocolVersion_1_0_0::get(),
	ProtocolVersion_0_0_0::get()
};


void Protocol::sendVersion(OutputStream *output, const ProtocolVersion *v)
{
	uint16_t numbers[3] = { v->major(), v->minor(), v->variant() };
	size_t i;

	for (i = 0; i < 3; i++)
		output->write(&numbers[i], sizeof (numbers[i]));
}

void Protocol::recvVersion(InputStream *input, uint16_t *major,
			   uint16_t *minor, uint16_t *variant)
{
	input->readall(major, sizeof (*major));
	input->readall(minor, sizeof (*minor));
	input->readall(variant, sizeof (*variant));
}

const ProtocolVersion *Protocol::version() const noexcept
{
	return ProtocolVersion_0_0_0::get();
}

void Protocol::exit() const
{
	throw ProtocolException();
}

void Protocol::push(const PushSettings &settings [[gnu::unused]]) const
{
	throw ProtocolException();
}

void Protocol::pull(const string &localRoot [[gnu::unused]],
		    const Reference &ref [[gnu::unused]]) const
{
	throw ProtocolException();
}

void Protocol::serve(Repository *repository [[gnu::unused]]) const
{
	throw ProtocolException();
}

unique_ptr<Protocol> Protocol::clientHandcheck(Channel *channel)
{
	uint16_t major, minor, variant;
	const ProtocolVersion *version;
	size_t i, len;
	bool found;

	len = sizeof (__versions) / sizeof (*__versions);

	for (i = 0; i < len; i++) {
		version = __versions[i];
		sendVersion(channel->outputStream(), version);
	}

	recvVersion(channel->inputStream(), &major, &minor, &variant);

	found = false;

	for  (i = 0; i < len; i++) {
		version = __versions[i];
		if (version->isCompatible(major, minor, variant)) {
			found = true;
			break;
		}
	}

	if (!found || (version == ProtocolVersion_0_0_0::get()))
		return nullptr;

	return version->getProtocol(channel);
}

unique_ptr<Protocol> Protocol::serverHandcheck(Channel *channel)
{
	uint16_t major, minor, variant;
	const ProtocolVersion *version;
	size_t i, len;
	bool found;

	len = sizeof (__versions) / sizeof (*__versions);
	i = 0;

	found = false;
	do {
		recvVersion(channel->inputStream(), &major, &minor, &variant);

		for  (i = 0; (i < len) && !found; i++) {
			version = __versions[i];
			if (version->isCompatible(major, minor, variant)) {
				found = true;
				break;
			}
		}
	} while((major != 0) || (minor != 0) || (variant != 0));

	sendVersion(channel->outputStream(), version);

	if (version == ProtocolVersion_0_0_0::get())
		return nullptr;

	return version->getProtocol(channel);
}
