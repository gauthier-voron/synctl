#ifndef _INCLUDE_SYNCTL_RECEIVE_1_HXX_
#define _INCLUDE_SYNCTL_RECEIVE_1_HXX_


#include "synctl/repo/Snapshot.hxx"


namespace synctl {


class InputStream;
class Reference;
class Repository;


class Receive_1
{
	struct Context
	{
		InputStream  *input;
		Repository   *repository;
		Reference    *reference;
	};


	bool _receiveEntry(const Context *context);

	void _receiveDirectory(const Context *context);

	void _receiveRegular(const Context *context);

	void _receiveSymlink(const Context *context);


 public:
	void receive(InputStream *input, Repository *repository,
		     Snapshot::Content *content);
};


}


#endif
