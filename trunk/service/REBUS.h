// REBUS.h: interface for the CREBUS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REBUS_H__916B7B93_D8C8_42F3_975C_B1DC1DD5A773__INCLUDED_)
#define AFX_REBUS_H__916B7B93_D8C8_42F3_975C_B1DC1DD5A773__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

extern LPCTSTR gpszProductSalt2;

#import "../core/rebus.tlb" named_guids rename_namespace("RebusTypeLib")
#import "../core/project.tlb" named_guids rename_namespace("ProjectTypeLib") \
	rename("Automation", "Automation2") rename("ScriptletCoClass", "ScriptletCoClass2")
#import "../core/plugin.tlb" named_guids rename_namespace("PluginTypeLib") \
	rename("Automation", "Plugin") rename("ScriptletCoClass", "PluginCoClass")

#include "../common/evaluation.h"

extern LPCTSTR gpszProductSalt1;

// 
struct tThreadBuildHandle {
	HANDLE handle;
	DWORD  dwThreadId;

	tThreadBuildHandle(HANDLE a_handle, DWORD a_id) : 
		handle(a_handle), dwThreadId(a_id) {}
};

typedef std::map<_bstr_t, tThreadBuildHandle> tThreadBuildMap;
typedef std::pair<_bstr_t, tThreadBuildHandle> tMapPair;

// thread function for build process
extern DWORD WINAPI ThreadBuildProject(LPVOID lpData);

//
class CREBUS  
{
	friend DWORD WINAPI ThreadBuildProject(LPVOID lpData);

private:
	tThreadBuildMap m_buildmap;
	CLogFile m_log;

public:
	CREBUS();
	virtual ~CREBUS();

public:
	class CRegistrationInfo
	{
		CCEvaluation14DaysExt m_eval;

	public:
		CRegistrationInfo(void) 
		{
			// set product salts
			m_eval.setProductSalt1(gpszProductSalt1);
			m_eval.setProductSalt2(gpszProductSalt2);

			// set user registration info
			CString szValue;
			registry.GetMachineEntry( "REBUS", "UserData", szValue);
			m_eval.setClientInfo( szValue );

			registry.GetMachineEntry( "REBUS", "ProductKey", szValue);
			m_eval.setProductKey( szValue );
		}

		~CRegistrationInfo() {}

	public:
		bool IsRegistered() {
			return m_eval.canUse();
		}
		DWORD GetEvaluationTimeLeft() {
			return m_eval.getEvaluationTimeLeft();
		}
		void setSatelliteCLSID(LPCTSTR lpszCLSID) {
			m_eval.setEvaluationCLSID( lpszCLSID );
		}
		void CheckProductRegistration() {
			// get product registration info
			m_eval.resetState();
		}
		void ResetRegistrationInfo( CString szUserData, CString szProductKey )
		{
			m_eval.setClientInfo( szUserData );
			m_eval.setProductKey( szProductKey );
			m_eval.resetState();
		}
	} *m_regInfo;

public:
	_bstr_t getBuildPriority(_bstr_t project_id);
	void changeBuildPriority(_bstr_t project_id, int direction);
	void init(CLogFile &log);
	_bstr_t getRootPath();
	_bstr_t getPage( LPCTSTR name );
	MSXML2::IXMLDOMDocumentPtr getPluginParmsDef( _bstr_t coclass );
	MSXML2::IXMLDOMDocumentPtr getProjectConfig(long index);
	long getPluginCount();
	_bstr_t getPlugin(long index);
	void saveConfig();
	MSXML2::IXMLDOMDocumentPtr config();
	void removeProject(_bstr_t codename);
	_bstr_t getTemplate(long index);
	long getTemplateCount();
	long CreateProject(_bstr_t codename, _bstr_t path, _bstr_t product,
						_bstr_t description, _bstr_t stemplate);
	ProjectTypeLib::Automation2Ptr getProject( long index );
	long getProjectsCount();
	void buildProject(_bstr_t id);

protected:
	_bstr_t m_rootpath;
	RebusTypeLib::AutomationPtr m_rebus;
};

extern CREBUS rebus;

#endif // !defined(AFX_REBUS_H__916B7B93_D8C8_42F3_975C_B1DC1DD5A773__INCLUDED_)
