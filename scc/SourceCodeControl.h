// SourceCodeControl.h : Declaration of the CSourceCodeControl

#ifndef __SOURCECODECONTROL_H_
#define __SOURCECODECONTROL_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSourceCodeControl
class ATL_NO_VTABLE CSourceCodeControl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSourceCodeControl, &CLSID_SourceCodeControl>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CSourceCodeControl>,
	public IDispatchImpl<ISourceCodeControl, &IID_ISourceCodeControl, &LIBID_REBUSSCCLib>
{
public:
	CSourceCodeControl() : m_user(""), m_password("")
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SOURCECODECONTROL)
DECLARE_NOT_AGGREGATABLE(CSourceCodeControl)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSourceCodeControl)
	COM_INTERFACE_ENTRY(ISourceCodeControl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CSourceCodeControl)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ISourceCodeControl
public:
	STDMETHOD(getFiles)(/*[in]*/ BSTR szProject, /*[in]*/ BSTR szLocal, /*[in]*/ BSTR szMask);
	STDMETHOD(connect)(/*[in]*/ BSTR szVendor, /*[in]*/ BSTR szUser, /*[in]*/ BSTR szPassword);
	STDMETHOD(getVendors)(/*[out, retval]*/ BSTR *szVendorList);

private:
	CSCCVendor m_scc;

private:
	_bstr_t m_password;
	_bstr_t m_user;
	HRESULT RaiseError( double number, _bstr_t description, _bstr_t source );
};

#endif //__SOURCECODECONTROL_H_
