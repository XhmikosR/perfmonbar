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

#include <unordered_map>
#include <vector>
#include <comdef.h>

class Configuration
{
public:
    struct Display {
        std::wstring Counter;
        std::wstring Prefix;
        std::wstring Suffix;
        int          Characters;
        int          Decimals;
        double       Add;
        double       Divide;

        Display()
            : Characters(0)
            , Decimals(0)
            , Add(0)
            , Divide(0) {
        }
    };

    struct Font {
        std::wstring Family;
        bool         Bold;
        bool         Italic;
        DWORD        Color;
        double       Size;

        Font()
            : Family(L"Arial")
            , Bold(true)
            , Italic(false)
            , Color(0x00FFFFFF)
            , Size(8.5) {
        }
    };

    struct Line {
        Font Font;
        std::vector<Display> Display;
    };

    struct Page {
        int               OffsetY;
        std::vector<Line> Lines;

        Page()
            : OffsetY(0) {
        }
    };

    struct Counter {
        std::wstring Name;
        std::wstring Value;
    };

    struct Settings {
        int MinSizeX;
        int MinSizeY;

        Settings()
            : MinSizeX(10)
            , MinSizeY(10) {
        }
    };

    typedef std::unordered_map<std::wstring, Counter> counters_t;
    typedef std::vector<Page> pages_t;
    typedef Settings settings_t;

private:
    counters_t _counters;
    pages_t    _pages;
    settings_t _settings;

    void Initialize();
    static bool ReadCounter(IXMLDOMNodePtr& node, Counter& counter);
    static bool ReadCounters(IXMLDOMNodePtr& node, counters_t& counters);
    static bool ReadPages(IXMLDOMNodePtr& node, std::vector<Page>& pages);
    static bool ReadPage(IXMLDOMNodePtr& node, Page& page);
    static bool ReadLines(IXMLDOMNodePtr& node, std::vector<Line>& lines);
    static bool ReadLine(IXMLDOMNodePtr& node, Line& line);
    static bool ReadDisplay(IXMLDOMNodePtr& node, Display& display);
    static bool ReadSettings(IXMLDOMNodePtr& node, settings_t& counters);

public:
    static HRESULT GetConfigPath(std::wstring& configPath);
    bool Read();

    counters_t& GetCounters() { return _counters; }
    pages_t& GetPages() { return _pages; }
    settings_t& GetSettings() { return _settings; }
};
