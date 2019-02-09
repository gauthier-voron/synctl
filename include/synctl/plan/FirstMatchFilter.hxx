#ifndef _INCLUDE_SYNCTL_FIRSTMATCHFILTER_HXX_
#define _INCLUDE_SYNCTL_FIRSTMATCHFILTER_HXX_


#include <memory>
#include <string>
#include <vector>

#include "synctl/plan/Filter.hxx"


namespace synctl {


class Pattern;


class FirstMatchFilter : public Filter
{
	using pattern_ptr = std::unique_ptr<Pattern>;
	using rule_t = std::pair<pattern_ptr, Filter::Action>;

	std::vector<rule_t>  _rules;


 public:
	FirstMatchFilter();
	FirstMatchFilter(const FirstMatchFilter &other) = default;
	virtual ~FirstMatchFilter();

	FirstMatchFilter &operator=(const FirstMatchFilter &other) = default;


	void append(std::unique_ptr<Pattern> pattern, Filter::Action action);

	virtual Filter::Action apply(const std::string &path);
};


}


#endif
