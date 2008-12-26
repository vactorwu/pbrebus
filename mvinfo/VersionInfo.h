// VersionInfo.h : Declaration of the CVersionInfo

#ifndef __VERSIONINFO_H_
#define __VERSIONINFO_H_

#include "resource.h"       // main symbols
#include "vinfo.h"

class CVIErrorHandler
{
public:
	void error(LPCTSTR lpszErrorText)
	{
	}
};

/////////////////////////////////////////////////////////////////////////////
// CVersionInfo
class ATL_NO_VTABLE CVersionInfo : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVersionInfo, &CLSID_VersionInfo>,
	public IDispatchImpl<IVersionInfo, &IID_IVersionInfo, &LIBID_MVINFOLib>
{
	vi::VersionInfo<vi::VIParserSimple, CVIErrorHandler> *pvi_;
	int mode;

public:
	CVersionInfo() : pvi_(NULL), mode(0)
	{
	}

	~CVersionInfo() {
		if(pvi_) delete pvi_;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VERSIONINFO)
DECLARE_NOT_AGGREGATABLE(CVersionInfo)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CVersionInfo)
	COM_INTERFACE_ENTRY(IVersionInfo)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVersionInfo
public:
	STDMETHOD(getProperty)(BSTR name, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(setProperty)(BSTR name, BSTR val);
	STDMETHOD(get_SpecialBuild)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_SpecialBuild)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_OriginalFilename)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_OriginalFilename)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_LegalTrademarks)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_LegalTrademarks)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Comments)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Comments)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ProductName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ProductName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_InternalName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_InternalName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_CompanyName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_CompanyName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Copyright)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Copyright)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Description)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Mode)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_Mode)(/*[in]*/ short newVal);
	STDMETHOD(get_ProductVersion)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ProductVersion)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FileVersion)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FileVersion)(/*[in]*/ BSTR newVal);
	STDMETHOD(load)(BSTR lpszFileName);
};

#endif //__VERSIONINFO_H_
