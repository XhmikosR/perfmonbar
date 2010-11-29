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

class Configuration
{
public:
    struct Display
    {
        tstring Counter;
        tstring Prefix;
        tstring Suffix;
        int Decimals;
        int Divide;

        Display()
        {
            Decimals = 0;
            Divide = 0;
        }
    };

    struct Font
    {
        tstring Family;
        bool    Bold;
        bool    Italic;
        DWORD   Color;
        double  Size;

        Font()
        {
            Family = _T("Arial");
            Bold = true;
            Italic = false;
            Color = 0x00FFFFFF;
            Size = 8.5;
        }
    };

    struct Line
    {
        Font Font;
        vector<Display> Display;
    };

    struct Page
    {
        int OffsetY;
        vector<Line> Lines;

        Page()
        {
            OffsetY = 0;
        }
    };

    struct Counter
    {
        tstring Name;
        tstring Value;
    };

    typedef hash_map<tstring, Counter> counters_t;
    typedef vector<Page> pages_t;

private:
    counters_t _counters;
    pages_t _pages;
    //bool _hasErrors;
    
    void Initialize();
    bool ReadCounter(IXMLDOMNodePtr & node, Counter & counter);
    bool ReadCounters(IXMLDOMNodePtr & node, counters_t & counters);
    bool ReadPages(IXMLDOMNodePtr & node, std::vector<Page> & pages);
    bool ReadPage(IXMLDOMNodePtr & node, Page & page);
    bool ReadLines(IXMLDOMNodePtr & node, std::vector<Line> & lines);
    bool ReadLine(IXMLDOMNodePtr & node, Line & line);
    bool ReadDisplay(IXMLDOMNodePtr & node, Display & display);

public:
    HRESULT GetConfigPath(tstring & configPath);
    bool Read();

    //bool HasErrors() { return _hasErrors; }

    counters_t & GetCounters() { return _counters; }
    pages_t & GetPages() { return _pages; }
};
