#ifndef _INCLUDE_SYNCTL_LINK_1_HXX_
#define _INCLUDE_SYNCTL_LINK_1_HXX_


#include <set>
#include <string>


namespace synctl {


class InputStream;
class OutputStream;
class Reference;


class Link_1
{
	std::set<std::string>  _locations;


	void _write(OutputStream *output) const;

	void _read(InputStream *input);


 public:
	Link_1() = default;
	Link_1(const Link_1 &other) = default;

	Link_1 &operator=(const Link_1 &other) = default;


	void addLocation(const std::string &name);

	void removeLocation(const std::string &name);

	const std::set<std::string> &getLocations() const;

	void clear();


	void write(OutputStream *output, Reference *ref = nullptr) const;

	void read(InputStream *input, Reference *ref = nullptr);
};


}


#endif
