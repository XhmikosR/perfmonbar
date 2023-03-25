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
#include <vector>
#include <shlobj.h>
#include <comdef.h>
#include "Configuration.h"

void Configuration::Initialize()
{
    static bool ole_initialized = false;
    if (!ole_initialized) {
        if (SUCCEEDED(CoInitialize(nullptr))) {
            ole_initialized = true;
        }

    }

    _counters.clear();
    _pages.clear();
}

bool Configuration::ReadCounter(IXMLDOMNodePtr& node, Counter& counter)
{
    IXMLDOMNamedNodeMapPtr pNamedNodeMap;
    HRESULT hr = node->get_attributes(&pNamedNodeMap);
    if (FAILED(hr)) {
        return false;
    }

    IXMLDOMNodePtr pAttribute;

    hr = pNamedNodeMap->getNamedItem(bstr_t("name"), &pAttribute);
    if (FAILED(hr)) {
        return false;
    }

    VARIANT counterName;
    hr = pAttribute->get_nodeValue(&counterName);
    if (FAILED(hr)) {
        return false;
    }

    counter.Name = bstr_t(variant_t(counterName));

    hr = pNamedNodeMap->getNamedItem(bstr_t("value"), &pAttribute);
    if (FAILED(hr)) {
        return false;
    }

    VARIANT counterValue;
    hr = pAttribute->get_nodeValue(&counterValue);
    if (FAILED(hr)) {
        return false;
    }

    counter.Value = bstr_t(variant_t(counterValue));

    return true;
}

bool Configuration::ReadCounters(IXMLDOMNodePtr& node, counters_t& counters)
{
    IXMLDOMNodePtr childNode;
    HRESULT hr = node->get_firstChild(&childNode);

    while (childNode != nullptr) {
        ATL::CComBSTR nodeName;
        hr = childNode->get_nodeName(&nodeName);
        if (FAILED(hr)) {
            return false;
        }

        if (nodeName == "counter") {
            Counter newCounter;
            if (ReadCounter(childNode, newCounter)) {
                counters[newCounter.Name] = newCounter;
            }
        }

        IXMLDOMNodePtr nextChildNode;
        hr = childNode->get_nextSibling(&nextChildNode);
        if (FAILED(hr)) {
            return false;
        }

        childNode = nextChildNode;
    }

    return true;
}

bool Configuration::ReadPages(IXMLDOMNodePtr& node, std::vector<Page>& pages)
{
    IXMLDOMNodePtr childNode;
    HRESULT hr = node->get_firstChild(&childNode);

    while (childNode != nullptr) {
        ATL::CComBSTR nodeName;
        hr = childNode->get_nodeName(&nodeName);
        if (FAILED(hr)) {
            return false;
        }

        if (nodeName == "page") {
            Page newPage;
            if (ReadPage(childNode, newPage)) {
                pages.push_back(newPage);
            }
        }

        IXMLDOMNodePtr nextChildNode;
        hr = childNode->get_nextSibling(&nextChildNode);
        if (FAILED(hr)) {
            return false;
        }

        childNode = nextChildNode;
    }

    return true;
}

bool Configuration::ReadPage(IXMLDOMNodePtr& node, Page& page)
{
    IXMLDOMNamedNodeMapPtr pNamedNodeMap;
    HRESULT hr = node->get_attributes(&pNamedNodeMap);
    if (FAILED(hr)) {
        return false;
    }

    long itemCount;
    hr = pNamedNodeMap->get_length(&itemCount);
    if (FAILED(hr)) {
        return false;
    }

    IXMLDOMNodePtr pAttribute;
    for (long i = 0; i < itemCount; ++i) {
        hr = pNamedNodeMap->get_item(i, &pAttribute);
        if (FAILED(hr)) {
            return false;
        }

        ATL::CComBSTR nodeName;
        hr = pAttribute->get_nodeName(&nodeName);
        if (FAILED(hr)) {
            return false;
        }

        VARIANT nodeValue;
        hr = pAttribute->get_nodeValue(&nodeValue);
        if (FAILED(hr)) {
            return false;
        }

        bstr_t name(nodeName);
        variant_t value(nodeValue);

        if (name == bstr_t("offsetY")) {
            page.OffsetY = atoi(bstr_t(value));
        }
    }

    IXMLDOMNodePtr childNode;
    hr = node->get_firstChild(&childNode);

    while (childNode != nullptr) {
        ATL::CComBSTR nodeName;
        hr = childNode->get_nodeName(&nodeName);
        if (FAILED(hr)) {
            return false;
        }

        if (nodeName == "lines") {
            ReadLines(childNode, page.Lines);
        }

        IXMLDOMNodePtr nextChildNode;
        hr = childNode->get_nextSibling(&nextChildNode);
        if (FAILED(hr)) {
            return false;
        }

        childNode = nextChildNode;
    }

    return true;
}

bool Configuration::ReadLines(IXMLDOMNodePtr& node, std::vector<Line>& lines)
{
    IXMLDOMNodePtr childNode;
    HRESULT hr = node->get_firstChild(&childNode);

    while (childNode != nullptr) {
        ATL::CComBSTR nodeName;
        hr = childNode->get_nodeName(&nodeName);
        if (FAILED(hr)) {
            return false;
        }

        if (nodeName == "line") {
            Line newLine;
            if (ReadLine(childNode, newLine)) {
                lines.push_back(newLine);
            }
        }

        IXMLDOMNodePtr nextChildNode;
        hr = childNode->get_nextSibling(&nextChildNode);
        if (FAILED(hr)) {
            return false;
        }

        childNode = nextChildNode;
    }

    return true;
}

bool Configuration::ReadLine(IXMLDOMNodePtr& node, Line& line)
{
    IXMLDOMNamedNodeMapPtr pNamedNodeMap;
    HRESULT hr = node->get_attributes(&pNamedNodeMap);
    if (FAILED(hr)) {
        return false;
    }

    long itemCount;
    hr = pNamedNodeMap->get_length(&itemCount);
    if (FAILED(hr)) {
        return false;
    }

    IXMLDOMNodePtr pAttribute;
    for (long i = 0; i < itemCount; ++i) {
        hr = pNamedNodeMap->get_item(i, &pAttribute);
        if (FAILED(hr)) {
            return false;
        }

        ATL::CComBSTR nodeName;
        hr = pAttribute->get_nodeName(&nodeName);
        if (FAILED(hr)) {
            return false;
        }

        VARIANT nodeValue;
        hr = pAttribute->get_nodeValue(&nodeValue);
        if (FAILED(hr)) {
            return false;
        }

        bstr_t name(nodeName);
        variant_t value(nodeValue);

        if (name == bstr_t("fontFamily")) {
            line.Font.Family = bstr_t(value);
        } else if (name == bstr_t("fontBold")) {
            line.Font.Bold = _wcsicmp(bstr_t(value), L"true") == 0;
        } else if (name == bstr_t("fontItalic")) {
            line.Font.Italic = _wcsicmp(bstr_t(value), L"true") == 0;
        } else if (name == bstr_t("fontColor")) {
            swscanf_s(bstr_t(value), L"%lX", &line.Font.Color);
        } else if (name == bstr_t("fontSize")) {
            line.Font.Size = atof(bstr_t(value));
        }
    }

    IXMLDOMNodePtr childNode;
    hr = node->get_firstChild(&childNode);

    while (childNode != nullptr) {
        ATL::CComBSTR nodeName;
        hr = childNode->get_nodeName(&nodeName);
        if (FAILED(hr)) {
            return false;
        }

        if (nodeName == "display") {
            Display newDisplay;
            if (ReadDisplay(childNode, newDisplay)) {
                line.Display.push_back(newDisplay);
            }
        }

        IXMLDOMNodePtr nextChildNode;
        hr = childNode->get_nextSibling(&nextChildNode);
        if (FAILED(hr)) {
            return false;
        }

        childNode = nextChildNode;
    }

    return true;
}

bool Configuration::ReadDisplay(IXMLDOMNodePtr& node, Display& display)
{
    IXMLDOMNamedNodeMapPtr pNamedNodeMap;
    HRESULT hr = node->get_attributes(&pNamedNodeMap);
    if (FAILED(hr)) {
        return false;
    }

    long itemCount;
    hr = pNamedNodeMap->get_length(&itemCount);
    if (FAILED(hr)) {
        return false;
    }

    IXMLDOMNodePtr pAttribute;
    for (long i = 0; i < itemCount; ++i) {
        hr = pNamedNodeMap->get_item(i, &pAttribute);
        if (FAILED(hr)) {
            return false;
        }

        ATL::CComBSTR nodeName;
        hr = pAttribute->get_nodeName(&nodeName);
        if (FAILED(hr)) {
            return false;
        }

        VARIANT nodeValue;
        hr = pAttribute->get_nodeValue(&nodeValue);
        if (FAILED(hr)) {
            return false;
        }

        bstr_t name(nodeName);
        variant_t value(nodeValue);

        if (name == bstr_t("prefix")) {
            display.Prefix = bstr_t(value);
        } else if (name == bstr_t("suffix")) {
            display.Suffix = bstr_t(value);
        } else if (name == bstr_t("counter")) {
            display.Counter = bstr_t(value);
        } else if (name == bstr_t("divide")) {
            display.Divide = atof(bstr_t(value));
        } else if (name == bstr_t("decimals")) {
            display.Decimals = atoi(bstr_t(value));
        } else if (name == bstr_t("characters")) {
            display.Characters = atoi(bstr_t(value));
        } else if (name == bstr_t("add")) {
            display.Add = atof(bstr_t(value));
        }
    }

    return true;
}

bool Configuration::ReadSettings(IXMLDOMNodePtr& node, settings_t& settings)
{
    IXMLDOMNamedNodeMapPtr pNamedNodeMap;
    HRESULT hr = node->get_attributes(&pNamedNodeMap);
    if (FAILED(hr)) {
        return false;
    }

    long itemCount;
    hr = pNamedNodeMap->get_length(&itemCount);
    if (FAILED(hr)) {
        return false;
    }

    IXMLDOMNodePtr pAttribute;
    for (long i = 0; i < itemCount; ++i) {
        hr = pNamedNodeMap->get_item(i, &pAttribute);
        if (FAILED(hr)) {
            return false;
        }

        ATL::CComBSTR nodeName;
        hr = pAttribute->get_nodeName(&nodeName);
        if (FAILED(hr)) {
            return false;
        }

        VARIANT nodeValue;
        hr = pAttribute->get_nodeValue(&nodeValue);
        if (FAILED(hr)) {
            return false;
        }

        bstr_t name(nodeName);
        variant_t value(nodeValue);

        if (name == bstr_t("minSizeX")) {
            settings.MinSizeX = atoi(bstr_t(value));
        }

        if (name == bstr_t("minSizeY")) {
            settings.MinSizeY = atoi(bstr_t(value));
        }
    }

    return true;
}

HRESULT Configuration::GetConfigPath(std::wstring& filePath)
{
    wchar_t path[MAX_PATH] = { 0 };
    HRESULT hr = SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, SHGFP_TYPE_CURRENT, path);

    if (FAILED(hr)) {
        return hr;
    }

    filePath = std::wstring(path) + L"\\PerfmonBar\\config.xml";
    return S_OK;
}

bool Configuration::Read()
{
    Initialize();

    IXMLDOMDocumentPtr pDomDocument;

    HRESULT hr = pDomDocument.CreateInstance(CLSID_DOMDocument);
    if (FAILED(hr)) {
        return false;
    }

    VARIANT_BOOL success;
    std::wstring configPath;
    hr = GetConfigPath(configPath);
    if (hr != S_OK) {
        return false;
    }

    hr = pDomDocument->load(variant_t(configPath.c_str()), &success);
    if (hr != S_OK) {
        return false;
    }

    // Root node of XML document
    IXMLDOMElementPtr pDocRoot;
    hr = pDomDocument->get_documentElement(&pDocRoot);
    if (FAILED(hr)) {
        return false;
    }

    ATL::CComBSTR name;
    hr = pDocRoot->get_nodeName(&name);
    if (FAILED(hr)) {
        return false;
    }

    if (name == "perfbar") {
        IXMLDOMNodePtr childNode;
        hr = pDocRoot->get_firstChild(&childNode);

        while (childNode != nullptr) {
            ATL::CComBSTR nodeName;
            hr = childNode->get_nodeName(&nodeName);
            if (FAILED(hr)) {
                return false;
            }

            if (nodeName == "counters") {
                ReadCounters(childNode, _counters);
            } else if (nodeName == "pages") {
                ReadPages(childNode, _pages);
            } else if (nodeName == "settings") {
                ReadSettings(childNode, _settings);
            }

            IXMLDOMNodePtr nextChildNode;
            hr = childNode->get_nextSibling(&nextChildNode);
            if (FAILED(hr)) {
                return false;
            }

            childNode = nextChildNode;
        }
    }

    return true;
}
