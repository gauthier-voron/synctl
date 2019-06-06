#ifndef _INCLUDE_SYNCTL_LOGGERPUSH0_1_HXX_
#define _INCLUDE_SYNCTL_LOGGERPUSH0_1_HXX_


#include <string>

#include "synctl/plan/LoggerPush_1.hxx"


namespace synctl {


class LoggerPush0_1 : public LoggerPush_1
{
 public:
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
