#ifndef _INCLUDE_SYNCTL_PROTOCOL_HXX_
#define _INCLUDE_SYNCTL_PROTOCOL_HXX_


#include <cstdint>
#include <memory>
#include <vector>


namespace synctl {


class Channel;
class InputStream;
class OutputStream;
class ProtocolVersion;
class Reference;
class Repository;


class Protocol
{
 private:
	static void sendVersion(OutputStream *out, const ProtocolVersion *v);
	static void recvVersion(InputStream *input, uint16_t *major,
				uint16_t *minor, uint16_t *variant);


 public:
	Protocol() = default;
	virtual ~Protocol() = default;


	virtual const ProtocolVersion *version() const noexcept;


	virtual void exit() const;


	struct PushSettings
	{
		// Root of the local tree to push
		std::string   localRoot;

		// Name of the remote branch to create a snapshot on
		std::string   branchName;

		// Reference of the newly created snapshot
		Reference    *snapshotReference;
	};

	virtual void push(const PushSettings &settings) const;

	virtual void pull(const std::string &localRoot, const Reference &ref)
		const;


	virtual void serve(Repository *repository) const;


	static std::unique_ptr<Protocol> clientHandcheck(Channel *channel);

	static std::unique_ptr<Protocol> serverHandcheck(Channel *channel);
};


}


#endif
