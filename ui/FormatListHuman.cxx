#include "synctl/ui/FormatListHuman.hxx"

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "synctl/io/AdapterOutputStream.hxx"
#include "synctl/io/OutputStream.hxx"


#define DATE_MAXLEN  128


using std::cout;
using std::make_pair;
using std::string;
using std::vector;
using synctl::AdapterOutputStream;
using synctl::FormatListHuman;
using synctl::OutputStream;


FormatListHuman::Date::Date(const struct timespec &_ts)
	: ts(_ts)
{
}

bool FormatListHuman::Date::operator<(const Date &other) const
{
	if (ts.tv_sec != other.ts.tv_sec)
		return (ts.tv_sec < other.ts.tv_sec);
	return (ts.tv_nsec < other.ts.tv_nsec);
}

void FormatListHuman::_printDate(const Date &date)
{
	char datebuf[DATE_MAXLEN];
	time_t now = ::time(NULL);
	struct tm datetm, nowtm;

	::localtime_r(&date.ts.tv_sec, &datetm);
	::localtime_r(&now, &nowtm);

	if (datetm.tm_year == nowtm.tm_year)
		::strftime(datebuf, sizeof (datebuf), "%b %e %H:%M", &datetm);
	else
		::strftime(datebuf, sizeof (datebuf), "%b %e %Y", &datetm);

	_output->writeStr(datebuf);
}

FormatListHuman::FormatListHuman()
	: _aoutput(cout), _output(&_aoutput)
{
}

FormatListHuman::FormatListHuman(OutputStream *output)
	: _output(output)
{
}

void FormatListHuman::load(const string &trunk, const string &snapshot,
			   const struct timespec &date,
			   const vector<string> &branches)
{
	auto tpair = _trunks.emplace(make_pair(trunk, SortedSnapshots()));
	auto &ss = tpair.first->second;
	auto dpair = ss.emplace(make_pair(date, Snapshots()));
	auto &s = dpair.first->second;

	s[snapshot] = branches;
}

void FormatListHuman::flush()
{
	bool firstTrunk = true;
	string sep;

	for (const auto &tpair : _trunks) {
		if (firstTrunk)
			firstTrunk = false;
		else
			_output->writeStr("\n");

		_output->writeStr(tpair.first);
		_output->writeStr(":\n");

		for (auto it = tpair.second.rbegin();
		     it != tpair.second.rend(); ++it) {

			for (const auto &spair : it->second) {
				_output->writeStr(spair.first);
				_output->writeStr("  ");

				_printDate(it->first);
				sep = "  ";

				for (const string &branch : spair.second) {
					_output->writeStr(sep);
					_output->writeStr(branch);
					sep = ", ";
				}

				_output->writeStr("\n");
			}
		}
	}
}
