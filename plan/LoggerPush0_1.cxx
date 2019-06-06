#include "synctl/plan/LoggerPush0_1.hxx"

#include <string>


using std::string;
using synctl::LoggerPush0_1;


void LoggerPush0_1::receiveReference()
{
}

void LoggerPush0_1::seekEntry(const string &rpath [[gnu::unused]],
			      size_t len [[gnu::unused]],
			      size_t total [[gnu::unused]])
{
}

void LoggerPush0_1::sendEntry(const string &rpath [[gnu::unused]],
			      size_t len [[gnu::unused]],
			      size_t total [[gnu::unused]])
{
}

void LoggerPush0_1::createSnapshot(const string &trunk [[gnu::unused]],
				   const string &snapshot [[gnu::unused]],
				   const string &branch [[gnu::unused]])
{
}
