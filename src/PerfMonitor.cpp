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

#include "stdafx.h"
#include <atlbase.h>
#include <hash_map>
#include <vector>
#include <pdh.h>
#include "PerfMonitor.h"
#include "PerfBar.h"

bool CPerfMon::Start(std::vector<stdext::pair<std::wstring, std::wstring>>& counters)
{
    Stop();

    PDH_STATUS pdhStatus = PdhOpenQuery(nullptr, 0, &_query);

    if (pdhStatus != ERROR_SUCCESS) {
        return false;
    }

    wchar_t szPathBuffer[PDH_MAX_COUNTER_PATH] = { 0 };
    for (auto it = counters.begin(); it != counters.end(); ++it) {
        wcscpy_s(szPathBuffer, _countof(szPathBuffer), it->second.c_str());

        pdhStatus = PdhValidatePathW(szPathBuffer);
        if (pdhStatus != ERROR_SUCCESS) {
            continue;
        }

        HCOUNTER counter = nullptr;
        pdhStatus = PdhAddCounterW(_query, szPathBuffer, 0, &counter);

        if (pdhStatus == ERROR_SUCCESS) {
            _counters.push_back(make_pair(it->first, counter));
        }
    }

    return true;
}

void CPerfMon::Stop()
{
    for (auto it = _counters.begin(); it != _counters.end(); ++it) {
        PdhRemoveCounter(it->second);
    }

    if (_query) {
        PdhCloseQuery(_query);
    }

    _query = nullptr;
    _counters.clear();
}

stdext::hash_map<std::wstring, double> CPerfMon::GetValues()
{
    PDH_STATUS pdhStatus = PdhCollectQueryData(_query);

    if (pdhStatus != ERROR_SUCCESS) {
        return stdext::hash_map<std::wstring, double>();
    }

    stdext::hash_map<std::wstring, double> values;

    for (auto it = _counters.begin(); it != _counters.end(); ++it) {
        PDH_FMT_COUNTERVALUE pdhCounterValue;
        pdhStatus = PdhGetFormattedCounterValue(it->second, PDH_FMT_DOUBLE, nullptr, &pdhCounterValue);

        if (pdhStatus == ERROR_SUCCESS) {
            values[it->first] = pdhCounterValue.doubleValue;
        }
    }

    return values;
}
