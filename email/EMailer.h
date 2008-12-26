// EMailer.h : Declaration of the CEMailer

#ifndef __EMAILER_H_
#define __EMAILER_H_

#include "resource.h"       // main symbols
#include "smtp.h"

/////////////////////////////////////////////////////////////////////////////
// CEMailer
class ATL_NO_VTABLE CEMailer : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEMailer, &CLSID_EMailer>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CEMailer>,
	public IDispatchImpl<IEMailer, &IID_IEMailer, &LIBID_REBUSMAILLib>
{
	CSMTPConnection m_conn;
	CSMTPMessage m_mess;

public:
	CEMailer()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EMAILER)
DECLARE_NOT_AGGREGATABLE(CEMailer)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CEMailer)
	COM_INTERFACE_ENTRY(IEMailer)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CEMailer)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
	HRESULT RaiseError( double number, _bstr_t description, _bstr_t source );

// IEMailer
public:
	STDMETHOD(Send)();
	STDMETHOD(SetBody)(/*[in]*/ BSTR szBody);
	STDMETHOD(SetSubject)(/*[in]*/ BSTR szSubject);
	STDMETHOD(SetTo)(/*[in]*/ BSTR szToAddress);
	STDMETHOD(SetFrom)(/*[in]*/ BSTR szFromAddress);
};

#endif //__EMAILER_H_
