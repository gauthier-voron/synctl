#ifndef _INCLUDE_SYNCTL_FORMATLISTHUMAN_HXX_
#define _INCLUDE_SYNCTL_FORMATLISTHUMAN_HXX_


#include <ctime>
#include <map>
#include <string>
#include <vector>

#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/ui/FormatList.hxx"


namespace synctl {


class OutputStream;


class FormatListHuman : public FormatList
{
	struct Date
	{
		struct timespec  ts;

		Date(const struct timespec &ts);

		bool operator<(const Date &other) const;
	};

	using Branches = std::vector<std::string>;
	using Snapshots = std::map<std::string, Branches>;
	using SortedSnapshots = std::map<Date, Snapshots>;


	AdapterOutputStream                      _aoutput;
	OutputStream                            *_output;
	std::map<std::string, SortedSnapshots>   _trunks;


	void _printDate(const Date &date);


 public:
	FormatListHuman();
	FormatListHuman(OutputStream *output);
	FormatListHuman(const FormatListHuman &other) = delete;
	virtual ~FormatListHuman() = default;

	virtual void load(const std::string &trunk,
			  const std::string &snapshot,
			  const struct timespec &date,
			  const std::vector<std::string> &branches) override;

	virtual void flush() override;
};


}


#endif
