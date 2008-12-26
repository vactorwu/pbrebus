// pbcompiler.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f pbcompilerps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "pbcompiler.h"

#include "pbcompiler_i.c"
#include "compiler.h"

#include "../common/evaluation.h"

CComModule _Module;

LPCTSTR gpszNotRegisteredText = "the product is NOT REGISTERED";

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_compiler, CCompiler)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_PBCOMPILERLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	CCEvaluation14DaysExt eval;
	eval.setEvaluationCLSID("{77765AE8-6757-4ad0-8741-FCC99DE0E0A4}");

	bool b_was_setup = eval.was_setup();

    // registers object, typelib and all interfaces in typelib
	ULONG ulResult = _Module.RegisterServer(TRUE);

	if(!b_was_setup) eval.setup();

    return ulResult;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}


