#ifndef _INCLUDE_SYNCTL_REFERENCE_HXX_
#define _INCLUDE_SYNCTL_REFERENCE_HXX_


#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <string>


namespace synctl {


class Reference
{
	uint8_t  _array[20];


 public:
	Reference();
	Reference(const Reference &other);


	Reference &operator=(const Reference &other);


	bool operator==(const Reference &other) const;
	bool operator!=(const Reference &other) const;
	bool operator<(const Reference &other) const;


	const uint8_t *data() const noexcept;
	uint8_t *data() noexcept;
	static size_t size() noexcept;


	static Reference fromHex(const std::string &hex);
	static const Reference &zero();
	std::string toHex() const;
};


}


#endif
