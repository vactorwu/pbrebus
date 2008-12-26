// compiler.h : Declaration of the CCompiler

#ifndef __COMPILER_H_
#define __COMPILER_H_

#include "resource.h"       // main symbols
#include "pbcompilerCP.h"
#include <vector>

#include "../common/evaluation.h"

extern LPCTSTR gpszProductSalt2;
extern LPCTSTR gpszProductSalt1;

/////////////////////////////////////////////////////////////////////////////
// CCompiler
class ATL_NO_VTABLE CCompiler : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCompiler, &CLSID_compiler>,
	public ISupportErrorInfo,
	public IDispatchImpl<IPBCompiler, &IID_IPBCompiler, &LIBID_PBCOMPILERLib>,
	public IProvideClassInfoImpl<&CLSID_compiler>,
	public CProxy_IcompilerEvents< CCompiler >,
	public IConnectionPointContainerImpl<CCompiler>
{
	class CRegistrationInfo
	{
		CCEvaluation14DaysExt m_eval;

	public:
		CRegistrationInfo(void) 
		{
			// set product salts
			m_eval.setProductSalt2(gpszProductSalt2);

			// set user registration info
			CString szValue;
			registry.GetMachineEntry( "REBUS", "UserData", szValue);
			m_eval.setClientInfo( szValue );

			registry.GetMachineEntry( "REBUS", "ProductKey", szValue);
			m_eval.setProductKey( szValue );

			m_eval.setProductSalt1(gpszProductSalt1);
		}

		~CRegistrationInfo() {}

	public:
		bool IsRegistered() {
			return m_eval.canUse();
		}
		void setSatelliteCLSID(LPCTSTR lpszCLSID) {
			m_eval.setEvaluationCLSID( lpszCLSID );
		}
		void CheckProductRegistration() {
			// get product registration info
			m_eval.resetState();
		}
	} *m_regInfo;

public:
	CCompiler() : m_binpath(""), m_applibrary(""), m_exepath(""),
		m_iconpath(""), m_appobject(""), m_appresource("")
	{
		m_regInfo = new CRegistrationInfo;
		m_regInfo->setSatelliteCLSID("{77765AE8-6757-4ad0-8741-FCC99DE0E0A4}");
	}

DECLARE_REGISTRY_RESOURCEID(IDR_COMPILER)
DECLARE_NOT_AGGREGATABLE(CCompiler)

DECLARE_PROTECT_FINAL_CONSTRUCT()

HRESULT FinalConstruct( )
   {
		log.Init(_Module.m_hInst);
		log.write("===========================================================");
		
		m_bInit = false;
		m_bSessionReady = false;

		m_liblist.CreateInstance(MSXML2::CLSID_DOMDocument30);

		m_regInfo->CheckProductRegistration();

		return S_OK;
   }

BEGIN_COM_MAP(CCompiler)
	COM_INTERFACE_ENTRY(IPBCompiler)
	COM_INTERFACE_ENTRY(IDispatch) 
	COM_INTERFACE_ENTRY(IProvideClassInfo) 
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

public:
	void FinalRelease()
	{
		m_liblist.Release();
	}

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IPBCompiler
public:
	STDMETHOD(Init)(/*[in]*/BSTR szPbVersion, /*[in]*/ BSTR szBinPath);

public:
	STDMETHOD(Regenerate)();
	STDMETHOD(RegenerateLibrary)(BSTR libname);
	STDMETHOD(BuildLibrary)(/*[in]*/ BSTR);
	STDMETHOD(SetLibraryList)(/*[in]*/ BSTR);

protected:
	bool m_bInit;
	std::vector<std::string> liblist_;
	std::vector<std::string> linkedlist_;

	MSXML2::IXMLDOMDocumentPtr m_liblist;

	_bstr_t m_binpath,
			m_applibrary,
			m_exepath,
			m_iconpath,
			m_appobject,
			m_appresource;

	enum tSessionKind { SESSION_REGENERATE, SESSION_BUILD};

protected:
	bool IsLinkedLibrary( _bstr_t name );
	bool m_bSessionReady;
	bool buildExe();
	bool buildAll();
	bool buildLibrary(_bstr_t &name);
	void RegenerateAll();
	bool InitializeSession( tSessionKind kind = SESSION_REGENERATE );

	void RegenerateObject(_bstr_t &path, _bstr_t &name, _bstr_t &type ); 
	void RegenerateLibrary(_bstr_t &name);
	bool Migrate_();
	bool FullRebuild_();
	CString getDisplayError(int iCode);

public :
	STDMETHOD(SetApplicationResourceFile)(/*[in]*/ BSTR szApplicationResourceFile);
	STDMETHOD(SetApplicationObject)(/*[in]*/ BSTR szApplicationObject);
	STDMETHOD(SetIconFile)(/*[in]*/ BSTR szIconFile);
	STDMETHOD(SetExeName)(/*[in]*/ BSTR szExeName);
	STDMETHOD(SetApplicationLibrary)(/*[in]*/ BSTR szAppLibrary);
	STDMETHOD(SetLinkedLibrary)(/*[in]*/ BSTR szLibList);
	STDMETHOD(FullRebuild)();
	STDMETHOD(Migrate)();
	STDMETHOD(Build)();
	HRESULT error(CString &what);
	HRESULT error(LPCSTR what);
	void out(CString &what);
	void out(LPCSTR what);
	HRESULT RaiseError(double number, _bstr_t description, 
						_bstr_t source, _bstr_t helpFile, 
						unsigned long helpcontext);

BEGIN_CONNECTION_POINT_MAP(CCompiler)
	CONNECTION_POINT_ENTRY(DIID__IcompilerEvents)
END_CONNECTION_POINT_MAP()

};

extern LPCTSTR gpszNotRegisteredText;

//// compiler structures

struct _compile_error 
{
	CString szmsgnum, szmsgtxt;
	int ilevel, icolnum, ilinenum;
};

typedef std::vector<CString> _linker_errors;

#endif //__COMPILER_H_
