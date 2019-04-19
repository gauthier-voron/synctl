#ifndef _INCLUDE_SYNCTL_LINKTABLE_1_HXX_
#define _INCLUDE_SYNCTL_LINKTABLE_1_HXX_


#include <set>

#include "synctl/tree/Reference.hxx"


namespace synctl {


class InputStream;
class OutputStream;


class Linktable_1
{
	std::set<Reference>  _links;


	void _write(OutputStream *output) const;

	void _read(InputStream *input);


 public:
	Linktable_1() = default;
	Linktable_1(const Linktable_1 &other) = default;

	Linktable_1 &operator=(const Linktable_1 &other) = default;


	void addLink(const Reference &reference);

	void removeLink(const Reference &reference);

	const std::set<Reference> &getLinks() const;

	size_t size() const;


	void write(OutputStream *output, Reference *ref = nullptr) const;

	void read(InputStream *input, Reference *ref = nullptr);
};


}


#endif
