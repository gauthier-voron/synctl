#ifndef _INCLUDE_SYNCTL_LOGGERPUSH_1_HXX_
#define _INCLUDE_SYNCTL_LOGGERPUSH_1_HXX_


#include <string>


namespace synctl {


class LoggerPush_1
{
 public:
	virtual void receiveReference() = 0;

	virtual void seekEntry(const std::string &rpath, size_t len,
			       size_t total) = 0;

	virtual void sendEntry(const std::string &rpath, size_t len,
			       size_t total) = 0;

	virtual void createSnapshot(const std::string &trunk,
				    const std::string &snapshot,
				    const std::string &branch) = 0;
};


}


#endif
