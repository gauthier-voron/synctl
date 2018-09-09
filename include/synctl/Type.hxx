#ifndef _INCLUDE_SYNCTL_TYPE_HXX_
#define _INCLUDE_SYNCTL_TYPE_HXX_


#include <cstdint>


namespace synctl {


enum class Type : uint16_t
{
	None        = 0,
	DirectoryV1 = 1,
	RegularV1   = 2,
	SymlinkV1   = 3
};


}


#endif

