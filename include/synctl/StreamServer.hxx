#ifndef _INCLUDE_SYNCTL_STREAMSERVER_HXX_
#define _INCLUDE_SYNCTL_STREAMSERVER_HXX_


#include <cstdint>


namespace synctl {


class InputStream;
class OutputStream;
class Repository;


class StreamServer
{
	Repository    *_repository;
	InputStream   *_input;
	OutputStream  *_output;


	void _servePushV1();
	void _servePullV1();


 public:
	using Opcode = uint8_t;

	static const Opcode REPLY_OK        = 0x00;
	static const Opcode REPLY_FAILURE   = 0x01;


	StreamServer();
	StreamServer(Repository *repository, InputStream *input,
		     OutputStream *output);

	void serve();
};


}


#endif
