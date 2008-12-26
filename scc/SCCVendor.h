// SCCVendor.h: interface for the CSCCVendor class.
//
// © Evgeny Savitsky, 2002, init-s.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCCVENDOR_H__8531160E_590E_4267_BFCA_6FB2A83B39E2__INCLUDED_)
#define AFX_SCCVENDOR_H__8531160E_590E_4267_BFCA_6FB2A83B39E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pubscc.h"
#include <vector>
#include <string>

struct _vssItemInfo
{
	std::string path;
	bool bCheckedOut;

	_vssItemInfo() : bCheckedOut(false) {;} 
};

class CSCCException
{
public:
	std::string m_message,
				m_source;
public:
	CSCCException(std::string what, std::string source = "") {
		m_message = what;
		m_source = source;
	}
	CSCCException(const CSCCException& ex) {
		m_message = ex.m_message;
		m_source = ex.m_source;
	}
	CSCCException& operator=(const CSCCException& ex) {
		m_message = ex.m_message;
		m_source = ex.m_source;
		return *this;
	}

};

class CSCCVendor  
{
	bool	m_bInit;
	HMODULE	m_hVendorDll;

	void	*m_vendorContext;
	DWORD	m_checkoutCommentLength;
	DWORD	m_commentLength;
	LONG	m_Caps;
	char	m_auxPathLabel[SCC_max_path];
	char	m_SccVendorName[SCC_max_path];
	LONG	m_lastError;

public:
	void itemInfoToPathList(std::vector<_vssItemInfo> &array,
		std::vector<std::string> &items);

	bool SccCloseProject();
	bool HasAdvancedOptions(HWND, SCC_command);
	std::string GetErrorText();
	void GetErrorText(LPTSTR lpszText);
	CSCCVendor();
	virtual ~CSCCVendor();

	bool IsVendorSupportCap(long);
	bool SelectVendor(LPCSTR lpszVendorName);
	
	bool EnumVendors(std::vector<std::string> &, long &);
	
	// source code control interface by Microsoft Corp.
	bool SccInitialize(HWND, LPTSTR);
	bool SccUninitialize();
	long SccGetCommandOptions(HWND, SCC_command, LPVOID *);
	bool SccGetProjPath(HWND hWnd, LPSTR lpUser, LPSTR lpProjName, 
						LPSTR lpLocalPath, LPSTR lpAuxProjPath, 
						BOOL bAllowChangePath, BOOL *pbNew);
	bool SccOpenProject(HWND window, LPSTR user, 
						LPSTR project, LPSTR local_proj, LPSTR aux_proj, 
						LPSTR comment, LONG flags);
	bool SccPopulateList(SCC_command icmd, LONG nFiles, LPSTR *lpFileNames, 
						 std::vector<_vssItemInfo> &, LPLONG lpStatus, LONG fOptions);
	bool SccCheckin(HWND hWnd, std::vector<LPTSTR> &array, LPSTR lpComment, 
					LONG fOptions, LPVOID pbOptions);
	bool SccCheckout(HWND hWnd, std::vector<LPTSTR> &array, LPSTR lpComment, 
					 LONG fOptions, LPVOID pbOptions);
	bool SccUncheckout(HWND hWnd, std::vector<LPTSTR> &array, LONG fOptions, 
					   LPVOID pbOptions);
	bool SccGetLatest(HWND hWnd, std::vector<LPTSTR> &array, 
							   LONG fOptions, LPVOID pbOptions);

protected:
	bool getVendorDllPath(LPCSTR, LPTSTR);
};

// from PowerBuilder APIs and Interfaces.doc
typedef LONG (*LPTEXTOUTPROC) (LPSTR, LONG); 

#endif // !defined(AFX_SCCVENDOR_H__8531160E_590E_4267_BFCA_6FB2A83B39E2__INCLUDED_)
