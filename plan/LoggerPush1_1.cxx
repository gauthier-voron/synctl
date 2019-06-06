#include "synctl/plan/LoggerPush1_1.hxx"

#include <chrono>
#include <iostream>
#include <string>

#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/io/LogOutputStream.hxx"
#include "synctl/io/OutputStream.hxx"
#include "synctl/tree/Reference.hxx"

#define ENTRY_PATH_MAXLEN   42


using std::chrono::high_resolution_clock;
using std::cout;
using std::move;
using std::string;
using std::to_string;
using synctl::AdapterOutputStream;
using synctl::LoggerPush1_1;
using synctl::LogOutputStream;
using synctl::OutputStream;
using synctl::Reference;


static std::string __prefix(size_t *size) {
	if (*size < 1024)
		return "";
	*size /= 1024;

	if (*size < 1024)
		return "Ki";
	*size /= 1024;

	if (*size < 1024)
		return "Mi";
	*size /= 1024;

	if (*size < 1024)
		return "Gi";
	*size /= 1024;

	return "Ti";
}

LoggerPush1_1::Speedometer::Speedometer(size_t smooth)
	: _length(1 + smooth)
{
}

void LoggerPush1_1::Speedometer::newWindow()
{
	_starts.push_back(high_resolution_clock::now());
	_counts.push_back(0);

	if (_starts.size() > _length) {
		_starts.pop_front();
		_count -= _counts.front();
		_counts.pop_front();
	}
}

void LoggerPush1_1::Speedometer::increment(size_t len)
{
	_counts.back() += len;
	_count += len;
	_last = high_resolution_clock::now();
}

size_t LoggerPush1_1::Speedometer::perSecond() const
{
	auto diff = _last - _starts.front();
	size_t millis = std::chrono::duration_cast<std::chrono::milliseconds>
		(diff).count();

	if (millis == 0)
		return 0;
	return ((_count * 1000) / millis);
}

string LoggerPush1_1::_formatEntryPath(const string &path) const
{
	string ret = path;
	size_t pos = 0;

	if (ret.length() <= ENTRY_PATH_MAXLEN)
		return ret;

	while (ret.length() > (ENTRY_PATH_MAXLEN - 3)) {
		pos = ret.find("/", pos + 1);

		if (pos == string::npos)
			pos = ret.length() - (ENTRY_PATH_MAXLEN - 3);

		ret = ret.substr(pos);
	}

	return ("..." + ret);
}

void LoggerPush1_1::_receivedReferences()
{
	string prefix;
	size_t size;

	_cleanLine();

	_output->write("Received references: ");
	_output->write(to_string(_referenceCount));
	_output->write(", ");

	size = _referenceCount * Reference::size();
	prefix = __prefix(&size);
	_output->write(to_string(size));
	_output->write(" ");
	_output->write(prefix);

	_output->write("B | ");

	size = _referenceAverageSpeed.perSecond();
	prefix = __prefix(&size);
	_output->write(to_string(size));
	_output->write(" ");
	_output->write(prefix);

	_output->write("B/s\n");
	_output->flush();
}

void LoggerPush1_1::_sentEntries()
{
	string prefix;
	size_t size;

	_cleanLine();

	_output->write("Sent files: ");
	_output->write(to_string(_sendCount));
	_output->write("/");
	_output->write(to_string(_hashCount));
	_output->write(", ");

	size = _sendSize;
	prefix = __prefix(&size);
	_output->write(to_string(size));
	_output->write(" ");
	_output->write(prefix);

	_output->write("B | ");

	size = _sendAverageSpeed.perSecond();
	prefix = __prefix(&size);
	_output->write(to_string(size));
	_output->write(" ");
	_output->write(prefix);

	_output->write("B/s\n");
	_output->flush();
}

void LoggerPush1_1::_cleanLine()
{
	string buffer;
	size_t i;

	buffer.push_back('\r');
	for (i = 0; i < _lineLength; i++)
		buffer.push_back(' ');
	buffer.push_back('\r');

	_output->write(buffer);
	_lineLength = 0;
}

bool LoggerPush1_1::_refresh()
{
	auto now = high_resolution_clock::now();
	auto diff = now - _lastRefresh;

	if (diff < _refreshRate)
		return false;

	_lastRefresh = now;
	return true;
}

LoggerPush1_1::LoggerPush1_1(size_t refreshRate)
	: _aoutput(cout), _output(&_aoutput)
	, _refreshRate(std::chrono::milliseconds(refreshRate))
{
}

LoggerPush1_1::LoggerPush1_1(OutputStream *output, size_t refreshRate)
	: _output(output)
	, _refreshRate(std::chrono::milliseconds(refreshRate))
{
}

LoggerPush1_1::LoggerPush1_1(LoggerPush1_1 &&other)
	: _aoutput(move(other._aoutput)), _refreshRate(other._refreshRate)
	, _lastRefresh(other._lastRefresh)
	, _lineLength(other._lineLength)
	, _referenceAverageSpeed(move(other._referenceAverageSpeed))
	, _referenceInstantSpeed(move(other._referenceInstantSpeed))
	, _referenceCount(other._referenceCount)
	, _sendAverageSpeed(move(other._sendAverageSpeed))
	, _sendInstantSpeed(move(other._sendInstantSpeed))
	, _sendPath(other._sendPath)
	, _sendPreviousPath(other._sendPreviousPath)
	, _sendPathSize(other._sendPathSize)
	, _sendCount(other._sendCount)
	, _sendSize(other._sendSize)
	, _hashInstantSpeed(move(other._hashInstantSpeed))
	, _hashPath(other._hashPath)
	, _hashPreviousPath(other._hashPreviousPath)
	, _hashPathSize(other._hashPathSize)
	, _hashCount(other._hashCount)
{
	if (other._output == &other._aoutput)
		_output = &_aoutput;
	else
		_output = other._output;
}

LoggerPush1_1 &LoggerPush1_1::operator=(LoggerPush1_1 &&other)
{
	_aoutput = move(other._aoutput);
	_refreshRate = other._refreshRate;
	_lastRefresh = other._lastRefresh;
	_lineLength = other._lineLength;
	_referenceAverageSpeed = move(other._referenceAverageSpeed);
	_referenceInstantSpeed = move(other._referenceInstantSpeed);
	_referenceCount = other._referenceCount;
	_sendAverageSpeed = move(other._sendAverageSpeed);
	_sendInstantSpeed = move(other._sendInstantSpeed);
	_sendPath = other._sendPath;
	_sendPreviousPath = other._sendPreviousPath;
	_sendPathSize = other._sendPathSize;
	_sendCount = other._sendCount;
	_sendSize = other._sendSize;
	_hashInstantSpeed = move(other._hashInstantSpeed);
	_hashPath = other._hashPath;
	_hashPreviousPath = other._hashPreviousPath;
	_hashPathSize = other._hashPathSize;
	_hashCount = other._hashCount;

	if (other._output == &other._aoutput)
		_output = &_aoutput;
	else
		_output = other._output;

	return *this;
}

void LoggerPush1_1::receiveReference()
{
	LogOutputStream out;
	string prefix;
	size_t size;

	if (_referenceCount == 0) {
		_referenceAverageSpeed.newWindow();
		_referenceInstantSpeed.newWindow();
	}

	_referenceCount += 1;
	_referenceAverageSpeed.increment(Reference::size());
	_referenceInstantSpeed.increment(Reference::size());

	if (_refresh() == false)
		return;

	_cleanLine();

	out = LogOutputStream(_output);
	out.write("Receiving references: ");
	out.write(to_string(_referenceCount));
	out.write(", ");

	size = _referenceCount * Reference::size();
	prefix = __prefix(&size);
	out.write(to_string(size));
	out.write(" ");
	out.write(prefix);

	out.write("B | ");

	size = _referenceInstantSpeed.perSecond();
	prefix = __prefix(&size);
	out.write(to_string(size));
	out.write(" ");
	out.write(prefix);

	out.write("B/s");
	out.flush();

	_lineLength = out.count();

	_referenceInstantSpeed.newWindow();
}

void LoggerPush1_1::seekEntry(const string &rpath, size_t len, size_t total)
{
	LogOutputStream out;
	string prefix;
	size_t size;

	if ((_sendCount == 0) && (_hashCount == 0) && (_referenceCount > 0))
		_receivedReferences();

	if (_hashCount == 0)
		_hashInstantSpeed.newWindow();

	if (rpath != _hashPath) {
		_hashPath = rpath;
		_hashPathSize = 0;
		_hashCount += 1;
	}

	_hashPathSize += len;
	_hashInstantSpeed.increment(len);

	if (_refresh() == false)
		return;

	_cleanLine();

	out = LogOutputStream(_output);

	if (rpath == _hashPreviousPath) {
		out.write("Hashing file: ");
		out.write(_formatEntryPath(rpath));
		out.write(", ");

		if (total == 0) {
			size = _hashPathSize;
			prefix = __prefix(&size);
			_output->write(to_string(size));
			_output->write(" ");
			_output->write(prefix);
			_output->write("B");
		} else {
			size = (_hashPathSize * 100) / total;
			out.write(to_string(size));
			out.write("%");
		}

		size = _hashInstantSpeed.perSecond();
	} else {
		out.write("Sending files: ");
		out.write(to_string(_sendCount));
		out.write("/");
		out.write(to_string(_hashCount));
		out.write(", ");

		size = _sendSize;
		prefix = __prefix(&size);
		out.write(to_string(size));
		out.write(" ");
		out.write(prefix);

		out.write("B");

		size = _sendInstantSpeed.perSecond();
	}

	out.write(" | ");

	prefix = __prefix(&size);
	out.write(to_string(size));
	out.write(" ");
	out.write(prefix);

	out.write("B/s");
	out.flush();

	_lineLength = out.count();

	_hashInstantSpeed.newWindow();
	_hashPreviousPath = rpath;
}

void LoggerPush1_1::sendEntry(const string &rpath, size_t len, size_t total)
{
	LogOutputStream out;
	string prefix;
	size_t size;

	if ((_sendCount == 0) && (_hashCount == 0) && (_referenceCount > 0))
		_receivedReferences();

	if (_sendCount == 0) {
		_sendAverageSpeed.newWindow();
		_sendInstantSpeed.newWindow();
	}

	if (rpath != _sendPath) {
		_sendPath = rpath;
		_sendPathSize = 0;
		_sendCount += 1;
	}

	_sendPathSize += len;
	_sendSize += len;
	_sendAverageSpeed.increment(len);
	_sendInstantSpeed.increment(len);

	if (_refresh() == false)
		return;

	_cleanLine();

	out = LogOutputStream(_output);

	if (rpath == _sendPreviousPath) {
		out.write("Sending file: ");
		out.write(_formatEntryPath(rpath));
		out.write(", ");

		if (total == 0) {
			size = _sendPathSize;
			prefix = __prefix(&size);
			_output->write(to_string(size));
			_output->write(" ");
			_output->write(prefix);
			_output->write("B");
		} else {
			size = (_sendPathSize * 100) / total;
			out.write(to_string(size));
			out.write("%");
		}
	} else {
		out.write("Sending files: ");
		out.write(to_string(_sendCount));
		out.write("/");
		out.write(to_string(_hashCount));
		out.write(", ");

		size = _sendSize;
		prefix = __prefix(&size);
		out.write(to_string(size));
		out.write(" ");
		out.write(prefix);

		out.write("B");
	}

	out.write(" | ");

	size = _sendInstantSpeed.perSecond();
	prefix = __prefix(&size);
	out.write(to_string(size));
	out.write(" ");
	out.write(prefix);

	out.write("B/s");
	out.flush();

	_lineLength = out.count();

	_sendInstantSpeed.newWindow();
	_sendPreviousPath = rpath;
}

void LoggerPush1_1::createSnapshot(const string &trunk,
				   const string &snapshot,
				   const string &branch)
{
	if (_sendCount > 0)
		_sentEntries();

	_cleanLine();

	_output->write("Created snapshot: ");
	_output->write(trunk);
	_output->write("/");
	_output->write(snapshot);

	if (branch.empty() == false) {
		_output->write(" <- ");
		_output->write(branch);
	}

	_output->write("\n");
}
