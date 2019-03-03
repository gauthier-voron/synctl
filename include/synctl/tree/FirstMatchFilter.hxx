#ifndef _INCLUDE_SYNCTL_FIRSTMATCHFILTER_HXX_
#define _INCLUDE_SYNCTL_FIRSTMATCHFILTER_HXX_


#include <memory>
#include <string>
#include <vector>

#include "synctl/tree/Filter.hxx"


namespace synctl {


class InputStream;
class OutputStream;
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

	virtual Filter::Action apply(const std::string &path) override;

	virtual void write(OutputStream *output) const override;

	virtual void read(InputStream *input) override;
};


}


#endif
