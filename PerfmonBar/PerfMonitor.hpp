/*
    Copyright (C) 2008 Danny Couture

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

class CPerfMon
{
    typedef vector<pair<tstring, HCOUNTER>> counters_t;
private:
	HQUERY _query;
    counters_t _counters;

public:
	CPerfMon()
	{
		_query = NULL;
	}

	virtual ~CPerfMon()
	{
		Stop();
	}

    BOOL Start(vector<pair<tstring, tstring>> & counters);
	VOID Stop();
    hash_map<tstring, double> GetValues();
};