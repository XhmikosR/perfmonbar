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
#include "PerfBar.h"
#include "Configuration.hpp"

#define IDM_RELOAD 1
#define IDM_EDIT 2

STDMETHODIMP CPerfBar::FinalConstruct()
{
    m_currentPage = 0;
    ReloadConfiguration();
    return S_OK;
}

STDMETHODIMP CPerfBar::EditConfiguration()
{
    tstring configPath;
    HRESULT hr = m_config.GetConfigPath(configPath);

    if (FAILED(hr)) {
        return hr;
    }

    ShellExecute(NULL, _T("edit"), configPath.c_str(), NULL, NULL, SW_SHOWNORMAL);

    return S_OK;
}

STDMETHODIMP CPerfBar::ReloadConfiguration()
{
    m_perfMonitor.Stop();
    m_config.Read();
    std::vector<stdext::pair<tstring, tstring>> counterNames;
    Configuration::counters_t& counters = m_config.GetCounters();

    for (Configuration::counters_t::iterator it = counters.begin(); it != counters.end(); ++it) {
        counterNames.push_back(make_pair(it->first, it->second.Value));
    }

    m_perfMonitor.Start(counterNames);
    return S_OK;
}

STDMETHODIMP CPerfBar::GetBandInfo(DWORD dwBandID,
                                   DWORD dwViewMode,
                                   DESKBANDINFO* pdbi)
{
    UNREFERENCED_PARAMETER(dwBandID);
    UNREFERENCED_PARAMETER(dwViewMode);
    HRESULT hr = E_FAIL;

    if (pdbi) {
        if (pdbi->dwMask & DBIM_MINSIZE) {
            pdbi->ptMinSize.x = 10;
            pdbi->ptMinSize.y = 10;
        }

        if (pdbi->dwMask & DBIM_MAXSIZE) {
            pdbi->ptMaxSize.x = -1;
            pdbi->ptMaxSize.y = -1;
        }

        if (pdbi->dwMask & DBIM_INTEGRAL) {
            pdbi->ptIntegral.x = 1;
            pdbi->ptIntegral.y = 1;
        }

        if (pdbi->dwMask & DBIM_ACTUAL) {
            pdbi->ptActual.x = 0;
            pdbi->ptActual.y = 0;
        }

        if (pdbi->dwMask & DBIM_MODEFLAGS) {
            pdbi->dwModeFlags = DBIMF_VARIABLEHEIGHT | DBIF_VIEWMODE_TRANSPARENT;
        }

        /*if(pdbi->dwMask & DBIM_BKCOLOR)
            pdbi->dwMask &= ~DBIM_BKCOLOR;*/

        hr = S_OK;
    } else {
        hr = E_INVALIDARG;
    }

    return hr;
}

STDMETHODIMP CPerfBar::GetWindow(HWND* phWnd)
{
    HRESULT hr = S_OK;

    if (phWnd == NULL) {
        hr = E_INVALIDARG;
    } else {
        *phWnd = m_hWnd;
        hr     = S_OK;
    }

    return hr;
}

STDMETHODIMP CPerfBar::ContextSensitiveHelp(BOOL bHelp)
{
    UNREFERENCED_PARAMETER(bHelp);
    return E_NOTIMPL;
}

STDMETHODIMP CPerfBar::ShowDW(BOOL fShow)
{
    if (IsWindow()) {
        if (fShow) {
            ShowWindow(SW_SHOW);
        } else {
            ShowWindow(SW_HIDE);
        }
    }

    return S_OK;
}

STDMETHODIMP CPerfBar::CloseDW(DWORD dwReserved)
{
    UNREFERENCED_PARAMETER(dwReserved);
    ShowDW(FALSE);

    if (IsWindow()) {
        DestroyWindow();
    }

    return S_OK;
}

STDMETHODIMP CPerfBar::ResizeBorderDW(LPCRECT    prcBorder,
                                      IUnknown*  punkToolbarSite,
                                      BOOL       fReserved)
{
    UNREFERENCED_PARAMETER(prcBorder);
    UNREFERENCED_PARAMETER(punkToolbarSite);
    UNREFERENCED_PARAMETER(fReserved);
    return E_NOTIMPL;
}

STDMETHODIMP CPerfBar::CanRenderComposited(BOOL* pfCanRenderComposited)
{
    *pfCanRenderComposited = TRUE;
    return S_OK;
}

STDMETHODIMP CPerfBar::SetCompositionState(BOOL fCompositionEnabled)
{
    m_fCompositionEnabled = fCompositionEnabled;
    return S_OK;
}

STDMETHODIMP CPerfBar::GetCompositionState(BOOL* pfCompositionEnabled)
{
    *pfCompositionEnabled = m_fCompositionEnabled;
    return S_OK;
}

STDMETHODIMP CPerfBar::SetSite(IUnknown* punkSite)
{
    HRESULT                hr          = E_FAIL;
    ATL::CComQIPtr<IOleWindow> spOleWindow;
    RECT                   rect        = {0};
    HWND                   hWndParent  = NULL;

    if (punkSite != NULL) {
        spOleWindow = punkSite;

        if (spOleWindow != NULL) {
            hr = spOleWindow->GetWindow(&hWndParent);

            if (SUCCEEDED(hr)) {
                ::GetClientRect(hWndParent, &rect);
                Create(hWndParent, rect, NULL, WS_CHILD);
                m_spInputObjSite = punkSite;
                hr               = m_spInputObjSite ? S_OK : E_FAIL;
            }
        }
    }

    return hr;
}

STDMETHODIMP CPerfBar::GetSite(REFIID  riid,
                               LPVOID* ppvReturn)
{
    HRESULT hr = E_FAIL;

    if (ppvReturn == NULL) {
        hr = E_INVALIDARG;
    } else if (m_spInputObjSite != NULL) {
        *ppvReturn = NULL;
        hr = m_spInputObjSite->QueryInterface(riid, ppvReturn);
    }

    return hr;
}

STDMETHODIMP CPerfBar::GetCommandString(UINT_PTR idCmd,
                                        UINT     uFlags,
                                        UINT*    pwReserved,
                                        LPSTR    pszName,
                                        UINT     cchMax)
{
    UNREFERENCED_PARAMETER(idCmd);
    UNREFERENCED_PARAMETER(uFlags);
    UNREFERENCED_PARAMETER(pwReserved);
    UNREFERENCED_PARAMETER(pszName);
    UNREFERENCED_PARAMETER(cchMax);
    return S_OK;
}

STDMETHODIMP CPerfBar::InvokeCommand(LPCMINVOKECOMMANDINFO pici)
{
    HRESULT hr = S_OK;

    if (HIWORD(pici->lpVerb) != 0) {
        hr = E_INVALIDARG;
    } else {
        switch (LOWORD(pici->lpVerb)) {
            case IDM_RELOAD:
                hr = ReloadConfiguration();
                break;
            case IDM_EDIT:
                hr = EditConfiguration();
                break;
            default:
                hr = E_INVALIDARG;
        }
    }

    return hr;
}

STDMETHODIMP CPerfBar::QueryContextMenu(HMENU hMenu,
                                        UINT  indexMenu,
                                        UINT  idCmdFirst,
                                        UINT  idCmdLast,
                                        UINT  uFlags)
{
    UNREFERENCED_PARAMETER(idCmdLast);
    HRESULT hr = S_OK;

    if (CMF_DEFAULTONLY & uFlags) {
        hr = MAKE_HRESULT(SEVERITY_SUCCESS, 0, 0);
    } else {
        InsertMenu(hMenu, indexMenu, MF_SEPARATOR | MF_BYPOSITION, idCmdFirst, 0);
        InsertMenu(hMenu, indexMenu, MF_STRING    | MF_BYPOSITION, idCmdFirst + IDM_RELOAD, _T("Performance Monitor - (Reload Configuration)"));
        InsertMenu(hMenu, indexMenu, MF_STRING    | MF_BYPOSITION, idCmdFirst + IDM_EDIT, _T("Performance Monitor - (Edit Configuration)"));

        hr = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 3);
    }

    return hr;
}

STDMETHODIMP CPerfBar::GetClassID(LPCLSID pClassID)
{
    *pClassID = CLSID_PerfBar;
    return S_OK;
}

STDMETHODIMP CPerfBar::IsDirty()
{
    return S_FALSE;
}

STDMETHODIMP CPerfBar::Load(LPSTREAM pStream)
{
    UNREFERENCED_PARAMETER(pStream);
    return S_OK;
}

STDMETHODIMP CPerfBar::Save(LPSTREAM pStream,
                            BOOL     bClearDirty)
{
    UNREFERENCED_PARAMETER(pStream);
    UNREFERENCED_PARAMETER(bClearDirty);
    return S_OK;
}

STDMETHODIMP CPerfBar::GetSizeMax(ULARGE_INTEGER* pcbSize)
{
    if (pcbSize == NULL) {
        return E_INVALIDARG;
    }

    ULISet32(*pcbSize, 0);

    return S_OK;
}

LRESULT CPerfBar::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(bHandled);

    Invalidate();

    return 0;
}

LRESULT CPerfBar::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(bHandled);

    size_t pageCount = m_config.GetPages().size();
    m_currentPage = (++m_currentPage % pageCount);

    Invalidate();

    return 0;
}

void CPerfBar::PaintData(HDC hdc, POINT offset)
{
    Configuration::pages_t& pages = m_config.GetPages();
    //Configuration::counters_t & counters = m_config.GetCounters();

    if (pages.empty()) {
        return;
    }

    if (pages.size() <= m_currentPage) {
        m_currentPage = pages.size() - 1;
    }

    Configuration::Page& page = pages[m_currentPage];

    if (page.Lines.empty()) {
        return;
    }

    stdext::hash_map<tstring, double> values = m_perfMonitor.GetValues();

    TEXTMETRIC textMetric;
    GetTextMetrics(hdc, &textMetric);

    TCHAR buf[1024];
    TCHAR display[1024];

    for (size_t i = 0; i < page.Lines.size(); ++i) {
        buf[0] = 0;

        Configuration::Line& line = page.Lines[i];

        for (std::vector<Configuration::Display>::iterator iit = line.Display.begin(); iit != line.Display.end(); ++iit) {
            stdext::hash_map<tstring, double>::const_iterator value_it = values.find(iit->Counter);

            TCHAR formattedValue[256];
            if (value_it == values.end()) {
                _tcscpy_s(formattedValue, _countof(formattedValue), _T("[N/A]"));
            } else {
                double val = value_it->second;
                if (iit->Divide > 0) {
                    val /= iit->Divide;
                }

                TCHAR formattingString[256];
                _stprintf_s(
                    formattingString,
                    _countof(formattingString),
                    _T("%s%d%s"),
                    _T("%."),
                    iit->Decimals > 0 ? iit->Decimals : 0,
                    _T("f")
                );

                _stprintf_s(
                    formattedValue,
                    _countof(formattedValue),
                    formattingString,
                    val
                );
            }

            _stprintf_s(
                display,
                _countof(display),
                _T("%s%s%s"),
                iit->Prefix.c_str(),
                formattedValue,
                iit->Suffix.c_str()
            );

            _tcscat_s(buf, _countof(buf), display);
        }

        RECT rc;
        GetClientRect(&rc);

        int offsetY = page.OffsetY + ((int)i * (textMetric.tmHeight - textMetric.tmExternalLeading));

        rc.top += offsetY + offset.y;
        rc.bottom += offset.y;
        rc.left += offset.x;
        rc.right += offset.x;

        HFONT font =
            CreateFont(
                -MulDiv((int)line.Font.Size, GetDeviceCaps(hdc, LOGPIXELSY), 72),
                0,
                0,
                0,
                line.Font.Bold ? FW_BOLD : 0,
                line.Font.Italic,
                FALSE,
                FALSE,
                ANSI_CHARSET,
                0,
                0,
                0,
                0,
                line.Font.Family.c_str()
            );

        SetTextColor(hdc, line.Font.Color);
        HFONT oldFont = (HFONT)SelectObject(hdc, font);
        DrawText(hdc, buf, (int)_tcslen(buf), &rc, DT_LEFT | DT_TOP);
        SelectObject(hdc, oldFont);
        DeleteObject(font);
    }
}

LRESULT CPerfBar::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(bHandled);

    PAINTSTRUCT ps      = {0};
    RECT        rect    = {0};
    HDC         hdcMem  = NULL;
    HBITMAP     hbmMem  = NULL;
    HBITMAP     hbmOld  = NULL;
    HFONT       hfOld   = NULL;

    BeginPaint(&ps);

    HWND parent = GetParent();

    ::GetClientRect(parent, &rect);

    hdcMem = CreateCompatibleDC(ps.hdc);
    hbmMem = CreateCompatibleBitmap(ps.hdc,
                                    rect.right - rect.left,
                                    rect.bottom - rect.top);

    hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
    hfOld = (HFONT)SelectObject(hdcMem, m_font);

    SendMessage(parent, WM_ERASEBKGND, (WPARAM)hdcMem, 0);
    SendMessage(parent, WM_PAINT, (WPARAM)hdcMem, 0);

    SetBkMode(hdcMem, TRANSPARENT);
    SetTextColor(hdcMem, 0x00FFFFFF);

    RECT rc;
    GetClientRect(&rc);
    ClientToScreen(&rc);

    POINT offset;
    offset.x = 0;
    offset.y = 0;

    ::ClientToScreen(parent, &offset);

    offset.x = rc.left - offset.x;
    offset.y = rc.top - offset.y;

    PaintData(hdcMem, offset);

    BitBlt(
        ps.hdc,
        0,
        0,
        rc.right - rc.left,
        rc.bottom - rc.top,
        hdcMem,
        offset.x,
        offset.y,
        SRCCOPY);

    SelectObject(hdcMem, hfOld);
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);

    EndPaint(&ps);

    return 0;
}

LRESULT CPerfBar::OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(bHandled);
    return TRUE;
}

LRESULT CPerfBar::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    bHandled = FALSE;

    HDC hDC = GetDC();

    m_font =
        CreateFont(
            -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72),
            0,
            0,
            0,
            FW_BOLD,
            FALSE,
            FALSE,
            FALSE,
            ANSI_CHARSET,
            0,
            0,
            0,
            0,
            _T("Arial")
        );

    ReleaseDC(hDC);

    SetTimer(1000, 1000, NULL);

    return 0;
}

LRESULT CPerfBar::OnGoodBye(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    bHandled = FALSE;

    if (m_font) {
        DeleteObject(m_font);
        m_font = NULL;
    }

    return 0;
}

