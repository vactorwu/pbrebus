// csystem.h : Declaration of the CSystem

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSystem
class ATL_NO_VTABLE CSystem : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSystem, &CLSID_system>,
	public IDispatchImpl<Isystem, &IID_Isystem, &LIBID_SYSLEVELLib>
{
public:
	CSystem()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SYSTEM)
DECLARE_NOT_AGGREGATABLE(CSystem)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSystem)
	COM_INTERFACE_ENTRY(Isystem)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// Isystem
public:
	STDMETHOD(getComputerName)(/*[out, retval]*/ BSTR *pName);
};

#endif //__SYSTEM_H_
