#ifndef _INCLUDE_SYNCTL_SYMLINK_1_HXX_
#define _INCLUDE_SYNCTL_SYMLINK_1_HXX_


#include <string>


namespace synctl {


class InputStream;
class OutputStream;
class Reference;


class Symlink_1
{
	std::string  _target;


	void _write(OutputStream *output) const;

	void _read(InputStream *input);


 public:
	Symlink_1() = default;
	explicit Symlink_1(const std::string &target);
	Symlink_1(const Symlink_1 &other) = default;

	Symlink_1 &operator=(const Symlink_1 &other) = default;


	void setTarget(const std::string &target);
	std::string getTarget() const;


	void write(OutputStream *output, Reference *ref) const;

	void read(InputStream *input, Reference *ref);


	static Symlink_1 make(const std::string &path);
};


}


#endif
