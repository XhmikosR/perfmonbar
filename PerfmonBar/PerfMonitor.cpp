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

#include <stdafx.h>
#include "PerfMonitor.hpp"
#include "PerfBar.h"

BOOL CPerfMon::Start(vector<pair<tstring, tstring>> & counters)
{
    Stop();

    PDH_STATUS pdhStatus = 
        PdhOpenQuery(NULL, 0, &_query);

    if (pdhStatus != ERROR_SUCCESS)
        return FALSE;

    TCHAR szPathBuffer[1024];
    for (vector<pair<tstring, tstring>>::const_iterator it = counters.begin(); it != counters.end(); ++it)
    {
        _tcscpy_s(szPathBuffer, sizeof(szPathBuffer)/sizeof(TCHAR), it->second.c_str());

        pdhStatus = PdhValidatePath(szPathBuffer);
        if (pdhStatus != ERROR_SUCCESS)
            continue;

        HCOUNTER counter = 0;
        pdhStatus = PdhAddCounter(_query, szPathBuffer, 0, &counter) ;

        if (pdhStatus == ERROR_SUCCESS)
            _counters.push_back(make_pair(it->first, counter));
    }

    return TRUE;
}

VOID CPerfMon::Stop()
{
    for (counters_t::iterator it = _counters.begin(); it != _counters.end(); ++it)
        PdhRemoveCounter(it->second);

    if (_query)
        PdhCloseQuery(_query);

    _query = NULL;
    _counters.clear();
}

hash_map<tstring, double> CPerfMon::GetValues()
{
    PDH_STATUS pdhStatus = PdhCollectQueryData(_query);

    if (pdhStatus != ERROR_SUCCESS)
        return hash_map<tstring, double>();

    hash_map<tstring, double> values;

    for (counters_t::iterator it = _counters.begin(); it != _counters.end(); ++it)
    {
        PDH_FMT_COUNTERVALUE pdhCounterValue;
        pdhStatus = PdhGetFormattedCounterValue( 
            it->second, 
            PDH_FMT_DOUBLE,
            NULL, 
            &pdhCounterValue );

        if ( pdhStatus == ERROR_SUCCESS )
            values[it->first] = pdhCounterValue.doubleValue;
    }

    return values;
}
