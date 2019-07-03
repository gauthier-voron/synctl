#ifndef _INCLUDE_SYNCTL_PROFILESEEKER_HXX_
#define _INCLUDE_SYNCTL_PROFILESEEKER_HXX_


#include <string>
#include <vector>


namespace synctl {


class ProfileSeeker
{
	std::vector<std::string>  _paths;


	void _sanitize(std::string *path) const;


 public:
	ProfileSeeker() = default;
	explicit ProfileSeeker(const std::vector<std::string> &paths);
	ProfileSeeker(const ProfileSeeker &other) = default;
	ProfileSeeker(ProfileSeeker &&other) = default;

	ProfileSeeker &operator=(const ProfileSeeker &other) = default;
	ProfileSeeker &operator=(ProfileSeeker &&other) = default;

	void push(const std::string &paths);
	void push(const std::vector<std::string> &paths);

	void clear();

	bool seek(const std::string &name, std::string *dest) const;
	std::string seek(const std::string &name) const;

	const std::vector<std::string> &paths() const;
};


}


#endif
