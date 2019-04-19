#ifndef _INCLUDE_SYNCTL_PROTOCOL_HXX_
#define _INCLUDE_SYNCTL_PROTOCOL_HXX_


#include <cstdint>
#include <memory>
#include <vector>


namespace synctl {


class Channel;
class Filter;
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

		// Name of the remote trunk to create a snapshot on
		std::string   trunkName;

		// Filter to select what entries to push
		Filter       *filter;

		// Filled with the name of the newly created snapshot if not
		// set to nullptr
		std::string  *snapshotName;
	};

	virtual void push(const PushSettings &settings) const;


	struct PullSettings
	{
		// Root of the local tree to pull to
		std::string   localRoot;

		// Name of the remote trunk to pull from
		std::string   trunkName;

		// If not empty, name of the snapshot to pull
		// If empty, pull the latest snapshot
		std::string   snapshotName;

		// Filter to select what entries to pull
		Filter       *filter;
	};

	virtual void pull(const PullSettings &settings) const;


	virtual void serve(Repository *repository) const;


	static std::unique_ptr<Protocol> clientHandcheck(Channel *channel);

	static std::unique_ptr<Protocol> serverHandcheck(Channel *channel);
};


}


#endif
