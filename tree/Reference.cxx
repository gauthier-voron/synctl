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


#include "synctl/tree/Reference.hxx"

#include <ostream>
#include <stdexcept>
#include <string>


using std::invalid_argument;
using std::ostream;
using std::out_of_range;
using std::string;
using synctl::Reference;


Reference::Reference()
{
}

Reference::Reference(const Reference &other)
{
	size_t i;

	for (i = 0; i < sizeof (_array); i++)
		_array[i] = other._array[i];
}

Reference &Reference::operator=(const Reference &other)
{
	size_t i;

	for (i = 0; i < sizeof (_array); i++)
		_array[i] = other._array[i];

	return *this;
}


bool Reference::operator==(const Reference &other) const
{
	size_t i;

	for (i = 0; i < sizeof (_array); i++)
		if (_array[i] != other._array[i])
			return false;

	return true;
}

bool Reference::operator!=(const Reference &other) const
{
	return !(*this == other);
}

bool Reference::operator<(const Reference &other) const
{
	size_t i;

	for (i = 0; i < sizeof (_array); i++)
		if (_array[i] < other._array[i])
			return true;
		else if (_array[i] > other._array[i])
			return false;

	return false;
}

const uint8_t *Reference::data() const noexcept
{
	return _array;
}

uint8_t *Reference::data() noexcept
{
	return _array;
}

size_t Reference::size() noexcept
{
	return sizeof (_array);
}

Reference Reference::fromHex(const string &hex)
{
	Reference ref;
	unsigned char c0, c1;
	size_t i, j;

	if (hex.length() < (sizeof (_array) * 2))
		throw out_of_range(hex);

	for (i = 0, j = 0; i < sizeof (_array); i++) {
		c0 = hex[j++];
		c1 = hex[j++];

		if ((c0 >= '0') && (c0 <= '9'))
			c0 -= '0';
		else if ((c0 >= 'a') && (c0 <= 'f'))
			c0 = c0 + 10 - 'a';
		else if ((c0 >= 'A') && (c0 <= 'F'))
			c0 = c0 + 10 - 'A';
		else
			throw invalid_argument(hex);

		if ((c1 >= '0') && (c1 <= '9'))
			c1 -= '0';
		else if ((c1 >= 'a') && (c1 <= 'f'))
			c1 = c1 + 10 - 'a';
		else if ((c1 >= 'A') && (c1 <= 'F'))
			c1 = c1 + 10 - 'A';
		else
			throw invalid_argument(hex);

		ref._array[i] = ((c0 << 4) & 0xf0) | (c1 & 0xf);
	}

	return ref;
}

const Reference &Reference::zero()
{
	static bool initialized = false;
	static Reference ref;
	size_t i;

	if (initialized == false) {
		for (i = 0; i < sizeof (ref._array); i++)
			ref._array[i] = 0;
		initialized = true;
	}

	return ref;
}

string Reference::toHex() const
{
	unsigned char c;
	string acc;
	size_t i;

	acc.reserve(sizeof (_array) * 2);

	for (i = 0; i < sizeof (_array); i++) {
		c = (_array[i] >> 4) & 0xf;

		if (c < 10)
			acc.push_back(c + '0');
		else
			acc.push_back(c - 10 + 'a');

		c = _array[i] & 0xf;

		if (c < 10)
			acc.push_back(c + '0');
		else
			acc.push_back(c - 10 + 'a');
	}

	return acc;
}
