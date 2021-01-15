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

#include "resource.h" // main symbols
#include <shlobj.h>
#include <atlctl.h>
#include "PerfmonBar_i.h"
#include "PerfMonitor.h"
#include "Configuration.h"

// CPerfBar
class ATL_NO_VTABLE CPerfBar :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ATL::CComCoClass<CPerfBar, &CLSID_PerfBar>,
    public ATL::IDispatchImpl <IPerfBar, &IID_IPerfBar, &LIBID_PerfmonBarLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
    public IObjectWithSite,
    public IPersistStream,
    public IDeskBand2,
    public IContextMenu,
    public ATL::CWindowImpl<CPerfBar>
{
private:
    CPerfMon      m_perfMonitor;
    ATL::CComQIPtr<IInputObjectSite> m_spInputObjSite;
    HFONT         m_font;
    Configuration m_config;
    size_t        m_currentPage;
    BOOL          m_fCompositionEnabled;

public:
    CPerfBar()
        : m_font(nullptr)
        , m_currentPage(0)
        , m_fCompositionEnabled(FALSE) {}

    DECLARE_REGISTRY_RESOURCEID(IDR_PERFBAR)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CPerfBar)
    COM_INTERFACE_ENTRY(IPerfBar)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IObjectWithSite)
    COM_INTERFACE_ENTRY(IDeskBand)
    COM_INTERFACE_ENTRY(IDeskBand2)
    COM_INTERFACE_ENTRY(IPersist)
    COM_INTERFACE_ENTRY(IPersistStream)
    COM_INTERFACE_ENTRY(IDockingWindow)
    COM_INTERFACE_ENTRY(IOleWindow)
    COM_INTERFACE_ENTRY(IContextMenu)
    END_COM_MAP()

    BEGIN_CATEGORY_MAP(CPerfBar)
    IMPLEMENTED_CATEGORY(CATID_DeskBand)
    IMPLEMENTED_CATEGORY(CATID_CommBand)
    END_CATEGORY_MAP()

    STDMETHOD(FinalConstruct)();

    ///////////////////////////////////////////////////////////////
    // IDeskBand methods

    STDMETHOD(GetBandInfo)(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi);

    STDMETHOD(GetWindow)(HWND* phwnd);

    STDMETHOD(ContextSensitiveHelp)(BOOL bEnterMode);

    STDMETHOD(ShowDW)(BOOL bShow);
    STDMETHOD(CloseDW)(DWORD dwReserved);
    STDMETHOD(ResizeBorderDW)(LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL bReserved);

    STDMETHOD(CanRenderComposited)(BOOL* pfCanRenderComposited);
    STDMETHOD(SetCompositionState)(BOOL fCompositionEnabled);
    STDMETHOD(GetCompositionState)(BOOL* pfCompositionEnabled);

    ///////////////////////////////////////////////////////////////
    // IObjectWithSite methods

    STDMETHOD(SetSite)(IUnknown* pUnkSite);
    STDMETHOD(GetSite)(REFIID riid, LPVOID* ppvSite);

    ///////////////////////////////////////////////////////////////
    // IPersistStream methods

    STDMETHOD(GetClassID)(LPCLSID pClassID);
    STDMETHOD(IsDirty)();
    STDMETHOD(Load)(LPSTREAM pStream);
    STDMETHOD(Save)(LPSTREAM pStream, BOOL bClearDirty);
    STDMETHOD(GetSizeMax)(ULARGE_INTEGER* pcbSize);

    ////////////////////////////////////////////////////////////////
    // IContext Menu Methods

    STDMETHOD(GetCommandString)(UINT_PTR idCmd, UINT uFlags, UINT* pwReserved, LPSTR pszName, UINT cchMax);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO pici);
    STDMETHOD(QueryContextMenu)(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
    STDMETHOD(ReloadConfiguration)();
    STDMETHOD(EditConfiguration)();

    ///////////////////////////////////////////////////////////////
    // Windows Message Map

    BEGIN_MSG_MAP(CPerfBar)
    MESSAGE_HANDLER(WM_CREATE,      OnCreate)
    MESSAGE_HANDLER(WM_DESTROY,     OnGoodBye)
    MESSAGE_HANDLER(WM_PAINT,       OnPaint)
    MESSAGE_HANDLER(WM_TIMER,       OnTimer)
    MESSAGE_HANDLER(WM_ERASEBKGND,  OnEraseBackground)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    END_MSG_MAP()


    ////////////////////////////////////////////////////////////////
    // Message Handlers

    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGoodBye(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    /////////////////////////////////////////////////////////////////
    // CPerfBar Methods
    void PaintData(HDC hdc, POINT offset);
    void DestroyBar();
};

OBJECT_ENTRY_AUTO(__uuidof(PerfBar), CPerfBar)
