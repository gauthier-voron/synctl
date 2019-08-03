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


#ifndef _INCLUDE_SYNCTL_LOGGERPUSH1_1_HXX_
#define _INCLUDE_SYNCTL_LOGGERPUSH1_1_HXX_


#include <chrono>
#include <list>
#include <string>

#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/plan/LoggerPush_1.hxx"


namespace synctl {


class LoggerPush1_1 : public LoggerPush_1
{
	using dm = std::chrono::duration<uint64_t, std::milli>;
	using tm = std::chrono::time_point<std::chrono::high_resolution_clock>;


	class Speedometer
	{
		size_t             _length;
		std::list<tm>      _starts;
		std::list<size_t>  _counts;
		tm                 _last;
		size_t             _count = 0;


	 public:
		Speedometer(size_t smooth = 0);
		Speedometer(const Speedometer &other) = default;
		Speedometer(Speedometer &&other) = default;


		Speedometer &operator=(const Speedometer &other) = default;
		Speedometer &operator=(Speedometer &&other) = default;


		void newWindow();

		void increment(size_t len);

		size_t perSecond() const;
	};


	AdapterOutputStream   _aoutput;
	OutputStream         *_output;
	dm                    _refreshRate;

	tm  _lastRefresh = std::chrono::high_resolution_clock::from_time_t(0);

	size_t                _lineLength = 0;

	Speedometer           _referenceAverageSpeed;
	Speedometer           _referenceInstantSpeed = Speedometer(3);
	size_t                _referenceCount = 0;

	Speedometer           _sendAverageSpeed;
	Speedometer           _sendInstantSpeed = Speedometer(3);
	std::string           _sendPath;
	std::string           _sendPreviousPath;
	size_t                _sendPathSize;
	size_t                _sendCount = 0;
	size_t                _sendSize = 0;

	Speedometer           _hashInstantSpeed = Speedometer(3);
	std::string           _hashPath;
	std::string           _hashPreviousPath;
	size_t                _hashPathSize;
	size_t                _hashCount = 0;


	std::string _formatEntryPath(const std::string &path) const;

	void _receivedReferences();

	void _sentEntries();

	void _cleanLine();

	bool _refresh();

	void _updateEntry(const std::string &rpath, size_t len, size_t total,
			  size_t *opcount, size_t *opsize,
			  const std::string &opname);


 public:
	LoggerPush1_1(size_t refreshRate = 500);
	LoggerPush1_1(OutputStream *output, size_t refreshRate = 500);
	LoggerPush1_1(LoggerPush1_1 &&other);


	LoggerPush1_1 &operator=(LoggerPush1_1 &&other);


	virtual void receiveReference() override;

	virtual void seekEntry(const std::string &rpath, size_t len,
			       size_t total) override;

	virtual void sendEntry(const std::string &rpath, size_t len,
			       size_t total) override;

	virtual void createSnapshot(const std::string &trunk,
				    const std::string &snapshot,
				    const std::string &branch) override;
};


}


#endif
