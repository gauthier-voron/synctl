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


	struct ListResult
	{
		// Trunk of the snapshot
		std::string               trunk;

		// Name of the snapshot
		std::string               name;

		// Creation date of the snapshot
		struct timespec           date;

		// Possible branches referencing the snapshot
		std::vector<std::string>  branches;
	};

	struct ListSettings
	{
		// Pcre2 regex for the trunks to list (".*" to list all trunks)
		std::string               trunkRegex;

		// Pcre2 regex for the branches to list (".*" to list all)
		// Set to empty string to list every snapshots.'
		std::string               branchRegex;

		// Filled with the listing of the result.
		// Must be a valid address.
		std::vector<ListResult>  *results;
	};

	virtual void list(const ListSettings &settings) const;


	struct PushSettings
	{
		// Root of the local tree to push
		std::string   localRoot;

		// Name of the remote branch to update
		std::string   branchName;

		// Name of the remote trunk to create a snapshot on
		std::string   trunkName;

		// Filter to select what entries to push
		Filter       *filter;

		// Filled with the name of the newly created snapshot if not
		// set to nullptr
		std::string  *snapshotName;

		// Verbosity level, 0 or less is silent, other levels are
		// implementation specific
		int           verbosity;
	};

	virtual void push(const PushSettings &settings) const;


	struct PullSettings
	{
		// Root of the local tree to pull to
		std::string   localRoot;

		// Name of the remote branch to update (no update if empty)
		std::string   branchName;

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
