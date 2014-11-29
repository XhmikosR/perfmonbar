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
#include "resource.h"
#include "PerfmonBar_i.h"
#include "PerfBar.h"

ATL::CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_PerfBar, CPerfBar)
END_OBJECT_MAP()

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);
    if (dwReason == DLL_PROCESS_ATTACH) {
        _Module.Init(ObjectMap, hInstance, &LIBID_PerfmonBarLib);
        DisableThreadLibraryCalls(hInstance);
    } else if (dwReason == DLL_PROCESS_DETACH) {
        _Module.Term();
    }
    return TRUE;    // ok
}

// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow()
{
    return (_Module.GetLockCount() == 0) ? S_OK : S_FALSE;
}

// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer()
{
    return _Module.RegisterServer(TRUE);
}

// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer()
{
    return _Module.UnregisterServer(TRUE);
}
