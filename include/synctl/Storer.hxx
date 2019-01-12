#ifndef _INCLUDE_SYNCTL_STORER_HXX_
#define _INCLUDE_SYNCTL_STORER_HXX_


namespace synctl {


class Reference;
class StoreContext;


class Storer
{
 public:
	virtual ~Storer();

	virtual void store(Reference *dest, StoreContext *ctx) = 0;
};


}


#endif
