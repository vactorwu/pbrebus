// SCCVendor.cpp: implementation of the CSCCVendor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCCVendor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCCVendor::CSCCVendor() : m_vendorContext(NULL), m_checkoutCommentLength(0),
	m_Caps(0), m_hVendorDll(NULL), m_bInit(false), m_lastError(SCC_return_success),
	m_commentLength(0)
{

}

CSCCVendor::~CSCCVendor()
{
	if(m_hVendorDll) {
		SccUninitialize();
		FreeLibrary(m_hVendorDll);
	}
}

#define NUMFUNCTION	11

// scci functions names
static LPCSTR lpszFunctions[NUMFUNCTION] = {
	"SccInitialize",
	"SccUninitialize",
	"SccGetCommandOptions",
	"SccGetProjPath",
	"SccOpenProject",
	"SccPopulateList",
	"SccCheckin",
	"SccCheckout",
	"SccUncheckout",
	"SccCloseProject",
	"SccGet"
	};

// scci functions addresses
static LPVOID lpDllFunctions[NUMFUNCTION] = {0};

////////////////////////////////////////////////////////////////////////
//
bool CSCCVendor::SelectVendor(LPCSTR lpszVendorName)
{
	TCHAR vendorDllPath[_MAX_PATH] = {0};

	// map vendor name to it's SCC server (dll)
	if(!getVendorDllPath(lpszVendorName, vendorDllPath)) 
		throw CSCCException("Can't resolve vendor name to it's SCC dll path", "SelectVendor(1)");

	m_bInit = false;

	if(m_hVendorDll) FreeLibrary(m_hVendorDll);
	
	// dynamically load SCC vendor server
	m_hVendorDll = LoadLibrary(vendorDllPath);
	if(!m_hVendorDll) 
	{
		std::string mess("Can't load vendor's scc dll: ");
		mess += vendorDllPath;
		mess += ", system error: ";

		LPVOID lpMsgBuf;
		if (FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL ))
		{
			mess += (char *) lpMsgBuf;
			LocalFree( lpMsgBuf );
		}

		throw CSCCException(mess, "SelectVendor(2)");
	}

	// build table of function pointers for SCCI
	for(int i = 0; i < NUMFUNCTION; i++) {
		lpDllFunctions[i] = (LPVOID) GetProcAddress(m_hVendorDll, lpszFunctions[i]);

		if(!lpDllFunctions[i]) 
		{
			std::string szmess("Can't find ");
			szmess += lpszFunctions[i];
			szmess += " function in the vendor's scc dll";

			throw CSCCException(szmess , "SelectVendor(3)");
		}
	}

	m_bInit = true;

	return m_bInit;
}

////////////////////////////////////////////////////////////////////////
//
bool CSCCVendor::IsVendorSupportCap(long cap) {
	return (m_bInit && ((m_Caps & cap) == cap));
}

////////////////////////////////////////////////////////////////////////
// maps vendor name onto it's server dll path
//
bool CSCCVendor::getVendorDllPath(LPCSTR lpszProviderName, LPTSTR lpszDllPath)
{
	HKEY hProvidersKey = 0;
	LPCSTR lpszProviders = "SOFTWARE\\SourceCodeControlProvider\\InstalledSCCProviders";

	long ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszProviders, 0, 
		KEY_READ, &hProvidersKey);

	if (ret != ERROR_SUCCESS) return false;

	DWORD reserved = 0;
	DWORD type = 0;
	TCHAR lpszProviderKey[256] = {0};
	DWORD sz = 256;
	
	// get provider registry key by it's name
	ret = ::RegQueryValueEx(hProvidersKey, lpszProviderName, NULL, 
		&type, (BYTE *) lpszProviderKey, &sz);

	::RegCloseKey(hProvidersKey);

	// open scci provider registry key
	HKEY hProviderKey = 0;

	ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR) lpszProviderKey, 0,
		KEY_READ, &hProviderKey);

	if (ret != ERROR_SUCCESS) return false;

	reserved = type = 0;
	sz = MAX_PATH;

	// get provider dll path from the registry
	ret = ::RegQueryValueEx(hProviderKey, "SCCServerPath", NULL, 
		&type, (BYTE *) lpszDllPath, &sz);

	::RegCloseKey(hProviderKey);

	return ret == ERROR_SUCCESS;
}

////////////////////////////////////////////////////////////////////////
// returns a collection of vendor names installed on the machine
// and the index of the current vendor 
//
bool CSCCVendor::EnumVendors(std::vector<std::string> &names, long &current)
{
	HKEY hKey = 0;
	LPCSTR lpszSCCPInstalled = "SOFTWARE\\SourceCodeControlProvider\\InstalledSCCProviders";
	LPCSTR lpszSCCP = "SOFTWARE\\SourceCodeControlProvider";

	long ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszSCCP, 0, KEY_READ, &hKey);
	if (ret != ERROR_SUCCESS) return false;

	DWORD reserved = 0;
	DWORD type = 0;
	TCHAR lpszCurrentProvider[256] = {0};
	DWORD sz = sizeof(lpszCurrentProvider);

	ret = ::RegQueryValueEx(hKey, "ProviderRegKey", NULL, 
		&type, (BYTE *) lpszCurrentProvider, &sz);

	::RegCloseKey(hKey);

	ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszSCCPInstalled, 0, KEY_READ, &hKey);
	if (ret != ERROR_SUCCESS) return false;

	DWORD dwIndex = 0;
	LONG lResult = ERROR_SUCCESS;
	TCHAR lpszValueName[MAX_PATH] = {0}, lpszValue[MAX_PATH] = {0};
	DWORD dwValueName, dwValue;

	while (lResult == ERROR_SUCCESS) 
	{
		dwValueName = MAX_PATH;
		dwValue = MAX_PATH;

		lResult = ::RegEnumValue(hKey, dwIndex++, lpszValueName, &dwValueName,
			NULL, &type, (BYTE *) lpszValue, &dwValue);

		if(lResult == ERROR_NO_MORE_ITEMS) break;

		names.push_back(std::string(lpszValueName));

		// store index of the current system provider
		if(strcmp(lpszCurrentProvider, lpszValue) == 0) 
			current = names.size() - 1;

		strcpy(lpszValueName, "");
		strcpy(lpszValue, "");
	}

	::RegCloseKey(hKey);

	return true;
}

// checks if vendor supports an advanced options dialog box
bool CSCCVendor::HasAdvancedOptions(HWND hWnd, SCC_command icmd)
{
	return (SccGetCommandOptions(hWnd, icmd, NULL) == 0);
}

////////////////////////////////////////////////////////////////////////
// error messages helper functions
//
static TCHAR errmsgBuffer[SCC_max_path] = {0};
//static LONG msgStatus = 0;

static LONG textOutProcedure(LPSTR lpszText, LONG status)
{
	strcpy(errmsgBuffer, lpszText);
//	msgStatus = status;
	return 0; // SCC_MSG_RTN_OK
}

void CSCCVendor::GetErrorText(LPTSTR lpszText)
{
	// vendor support inside error handling
	if(m_Caps & SCC_cap_want_outproc == SCC_cap_want_outproc) 
	{
		if(strcmp(errmsgBuffer, "") != 0) {
			strcpy(lpszText, errmsgBuffer);
			return;
		}
	}

	LPTSTR lpMsgBuf;
	::FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		::GetLastError(),
		0, // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	switch(m_lastError) 
	{
	case SCC_return_unknown_project:
		strcpy(lpszText, "SCCI: Unknown SCC project.");
		break;

	case SCC_return_non_scc_file:
		strcpy(lpszText, "SCCI: The file is not under SCC control.");
		break;

	case SCC_return_not_supported:
		strcpy(lpszText, "SCCI: This operation is not supported.");
		break;
		
	case SCC_return_non_specific_error:
		sprintf(lpszText, "SCCI: None specific error. SYSTEM: %s", lpMsgBuf);
		break;

	default:
		sprintf(lpszText, "SCCI: Unknown error code = %X. SYSTEM: %s", 
			m_lastError, lpMsgBuf);
	}

	::LocalFree((LPVOID) lpMsgBuf);
}

std::string CSCCVendor::GetErrorText()
{
	TCHAR szText[MAX_PATH] = {0};
	GetErrorText(szText);
	return std::string(szText);
}

////////////////////////////////////////////////////////////////////////
// set up the version control system. This should be called before any
// other SCC calls other than SccGetVersion. 
//
bool CSCCVendor::SccInitialize(HWND hWnd, LPTSTR caller)
{
	if(!m_bInit) return false;

	m_lastError = (* ((FPSCCINITIALIZE ) lpDllFunctions[0]))(&m_vendorContext, 
		hWnd, caller, m_SccVendorName, &m_Caps, m_auxPathLabel, 
		&m_checkoutCommentLength, &m_commentLength);

	return m_lastError == SCC_return_success;
}


bool CSCCVendor::SccUninitialize()
{
	if(!m_bInit) return false;

	m_lastError = (* ((FPSCCUNINITIALIZE ) lpDllFunctions[1]))(m_vendorContext);
	return m_lastError == SCC_return_success;
}

long CSCCVendor::SccGetCommandOptions(HWND hWnd, SCC_command iCommand, 
									  LPVOID *ppvOptions)
{
	if(!m_bInit) return -1;

	return (* ((FPSCCGETCOMMANDOPTIONS ) lpDllFunctions[2]))
		(m_vendorContext, hWnd, iCommand, ppvOptions);
}

// it is called to obtain project path in vendor's terms.
// lpAuxProjPath will contain the path
//
// you don't need to call this function every time, although
// you need to store lpAuxProjPath somewhere in project configuration file
//
bool CSCCVendor::SccGetProjPath(HWND hWnd, LPSTR lpUser, LPSTR lpProjName, 
								LPSTR lpLocalPath, LPSTR lpAuxProjPath, 
								BOOL bAllowChangePath, BOOL *pbNew)
{
	if(!m_bInit) return false;

	m_lastError = (* ((FPSCCGETPROJPATH ) lpDllFunctions[3]))
		(m_vendorContext, hWnd, lpUser, lpProjName, lpLocalPath,
		 lpAuxProjPath, bAllowChangePath, pbNew);

	return m_lastError == SCC_return_success;
}

bool CSCCVendor::SccOpenProject(HWND window, LPSTR user, 
								LPSTR project, LPSTR local_proj, LPSTR aux_proj, 
								LPSTR comment, LONG flags)
{
	if(!m_bInit) return false;

	m_lastError = (* ((FPSCCOPENPROJECT ) lpDllFunctions[4]))
		(m_vendorContext, window, user, project, local_proj,
		 aux_proj, comment, textOutProcedure, flags);

	return m_lastError == SCC_return_success;
}

// populate list callback function
//
BOOL fnPopulateList (LPVOID callerdat, BOOL add_keep, LONG status, LPCSTR file)
{
	std::vector<_vssItemInfo> &array = *((std::vector<_vssItemInfo> *) callerdat);
	_vssItemInfo info;

	info.path = file;
	info.bCheckedOut = (status & 0x100A) == 0x100A || (status & 0xD) == 0xD;

	array.push_back(info);

	return TRUE;
}

bool CSCCVendor::SccPopulateList(SCC_command icmd, LONG nFiles, LPSTR *lpFileNames,
								 std::vector<_vssItemInfo> &array, LPLONG lpStatus, LONG fOptions)
{
	if(!m_bInit) return false;

	array.clear();

	m_lastError = (* ((FPSCCPOPULATELIST) lpDllFunctions[5]))
		(m_vendorContext, icmd, nFiles, lpFileNames, fnPopulateList,
		 (LPVOID) &array, lpStatus, fOptions);

	return m_lastError == SCC_return_success;
}


bool CSCCVendor::SccCheckin(HWND hWnd, std::vector<LPTSTR> &array, 
							LPSTR lpComment, LONG fOptions, LPVOID pbOptions)
{
	if(!m_bInit) return false;
/*
	// prepear LPSTR list
	LPCSTR *lpFileNames = (LPCSTR *)new LPSTR[array.size()];
	int i = 0;

	for(std::vector<std::string>::iterator it = array.begin(); it != array.end(); ++it, i++)
		lpFileNames[i] = (*it).c_str();
*/
	// checkin files in array
	m_lastError = (* ((FPSCCCHECKIN) lpDllFunctions[6]))
		(m_vendorContext, hWnd, array.size(), array.begin(), lpComment, fOptions, pbOptions);

	//delete lpFileNames;

	return m_lastError == SCC_return_success;
}

bool CSCCVendor::SccCheckout(HWND hWnd, std::vector<LPTSTR> &array, 
							LPSTR lpComment, LONG fOptions, LPVOID pbOptions)
{
	if(!m_bInit) return false;
/*
	// prepear LPSTR list
	LPCTSTR *lpFileNames = new LPCTSTR[array.size()];
	int i = 0;

	for(std::vector<std::string>::iterator it = array.begin(); it != array.end(); ++it, i++)
		lpFileNames[i] = (*it).c_str();

	// checkin files in array
	m_lastError = (* ((FPSCCCHECKOUT) lpDllFunctions[7]))
		(m_vendorContext, hWnd, array.size(), (LPTSTR *) lpFileNames, lpComment, fOptions, pbOptions);

	delete lpFileNames;
*/
	m_lastError = (* ((FPSCCCHECKOUT) lpDllFunctions[7]))
		(m_vendorContext, hWnd, array.size(), array.begin(), lpComment, fOptions, pbOptions);

	return m_lastError == SCC_return_success;
}

bool CSCCVendor::SccUncheckout(HWND hWnd, std::vector<LPTSTR> &array, 
							   LONG fOptions, LPVOID pbOptions)
{
	if(!m_bInit) return false;
/*
	// prepear LPSTR list
	LPCTSTR *lpFileNames = new LPCTSTR[array.size()];
	int i = 0;

	for(std::vector<std::string>::iterator it = array.begin(); it != array.end(); ++it, i++)
		lpFileNames[i] = (*it).c_str();

	// checkin files in array
	m_lastError = (* ((FPSCCUNCHECKOUT) lpDllFunctions[8]))
		(m_vendorContext, hWnd, array.size(), (LPTSTR *) lpFileNames, fOptions, pbOptions);

	delete lpFileNames;
*/
	m_lastError = (* ((FPSCCUNCHECKOUT) lpDllFunctions[8]))
		(m_vendorContext, hWnd, array.size(), array.begin(), fOptions, pbOptions);

	return m_lastError == SCC_return_success;
}

bool CSCCVendor::SccCloseProject()
{
	if(!m_bInit) return false;

	m_lastError = (* ((FPSCCCLOSEPROJECT) lpDllFunctions[9]))
		(m_vendorContext);

	return m_lastError == SCC_return_success;
}

bool CSCCVendor::SccGetLatest(HWND hWnd, std::vector<LPTSTR> &array, 
							   LONG fOptions, LPVOID pbOptions)
{
	if(!m_bInit) return false;
/*
	// prepear LPSTR list
	LPCTSTR *lpFileNames = new LPCTSTR[array.size()];
	int i = 0;

	for(std::vector<std::string>::iterator it = array.begin(); it != array.end(); ++it, i++)
		lpFileNames[i] = (*it).c_str();

	// checkin files in array
	m_lastError = (* ((FPSCCGET) lpDllFunctions[10]))
		(m_vendorContext, hWnd, array.size(), (LPTSTR *) lpFileNames, fOptions, pbOptions);

	delete lpFileNames;
*/
	m_lastError = (* ((FPSCCGET) lpDllFunctions[10]))
		(m_vendorContext, hWnd, array.size(), array.begin(), fOptions, pbOptions);

	return m_lastError == SCC_return_success;
}

void CSCCVendor::itemInfoToPathList(std::vector<_vssItemInfo> &array,
									std::vector<std::string> &items)
{
	items.clear();
	
	for(std::vector<_vssItemInfo>::iterator it = array.begin();
		it != array.end(); ++it)
	{
		items.push_back((*it).path);
	}
}
