// Copyright 2019 Gauthier Voron
//
// This file is part of Synctl
//
// Synctl is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Fundation, either version 3 of the License, or (at your option) any later
// version.
//
// Synctl is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANDABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// Synctl. If not, see <https://www.gnu.org/licenses/>.
//


#ifndef _INCLUDE_SYNCTL_OPTIONLAMBDA_HXX_
#define _INCLUDE_SYNCTL_OPTIONLAMBDA_HXX_


#include <functional>
#include <string>

#include "synctl/ui/Option.hxx"


namespace synctl {


class OptionLambda : public Option
{
	bool                                      _withArg;
	std::function<void(const std::string &)>  _lambdaWithArg;
	std::function<void()>                     _lambdaWithoutArg;


 public:
	OptionLambda(const std::string &longName,
		     std::function<void()> lambda);
	OptionLambda(char shortName, std::function<void()> lambda);
	OptionLambda(const std::string &longName, char shortName,
		     std::function<void()> lambda);

	OptionLambda(const std::string &longName,
		     std::function<void(const std::string &)> lambda);
	OptionLambda(char shortName,
		     std::function<void(const std::string &)> lambda);
	OptionLambda(const std::string &longName, char shortName,
		     std::function<void(const std::string &)> lambda);


	virtual bool acceptArgument() const override;

	virtual void affect() override;
	virtual void affect(const std::string &argument) override;
};


}


#endif
