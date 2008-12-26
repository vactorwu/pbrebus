// process.h : Declaration of the Cprocess

#ifndef __PROCESS_H_
#define __PROCESS_H_

#include "resource.h"       // main symbols
#include <Userenv.h>

/////////////////////////////////////////////////////////////////////////////
// Cprocess
class ATL_NO_VTABLE Cprocess : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<Cprocess, &CLSID_process>,
	public ISupportErrorInfo,
	public IDispatchImpl<Iprocess, &IID_Iprocess, &LIBID_SYSLEVELLib>
{
public:
	Cprocess()
	{
		memset(&m_profileInfo, 0, sizeof(m_profileInfo));
		m_hToken = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROCESS)
DECLARE_NOT_AGGREGATABLE(Cprocess)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(Cprocess)
	COM_INTERFACE_ENTRY(Iprocess)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

public:
	void FinalRelease()
	{
		unloadUserProfile();
	}

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// Iprocess
public:
	STDMETHOD(setProcessPriority)(/*[in]*/ LONG lPriority);
	STDMETHOD(isProcessAlive)(/*[in]*/ LONG dwProcessId, BOOL *bResult);
	STDMETHOD(unloadUserProfile)();
	STDMETHOD(loadUserProfile)();
	STDMETHOD(getCurrentProcessId)(/*[out, retval]*/ LONG *pPID);
	STDMETHOD(kill)(/*[in]*/ LONG dwProcessId, /*[out, retval]*/ LONG *result);
	STDMETHOD(get)(/*[in]*/ BSTR name, /*[out, retval]*/ LONG *dwProcessId);

private:
	PROFILEINFO m_profileInfo;
	HANDLE		m_hToken;
};

#endif //__PROCESS_H_
