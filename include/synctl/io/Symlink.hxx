#ifndef _INCLUDE_SYNCTL_SYNLINK_HXX_
#define _INCLUDE_SYNCTL_SYNLINK_HXX_


namespace synctl {


class Symlink
{
	std::string  _path;


 public:
	Symlink() = default;
	Symlink(const std::string &path);
	Symlink(Symlink &&other) = default;

	Symlink &operator=(Symlink &&other) = default;


	bool exists() const;

	std::string get() const;


	void remove();

	void create(const std::string &target);

	void set(const std::string &target);
};


}


#endif
