// process.cpp : Implementation of Cprocess
#include "stdafx.h"
#include "Syslevel.h"
#include "process.h"

#include <windows.h>
#include <comdef.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <Userenv.h>

#include "..\common\cstring.h"
/////////////////////////////////////////////////////////////////////////////
// Cprocess

STDMETHODIMP Cprocess::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_Iprocess
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

BOOL GetProcessModule (DWORD dwPID, DWORD dwModuleID, 
     LPMODULEENTRY32 lpMe32, DWORD cbMe32) 
{ 
    BOOL          bRet        = FALSE; 
    BOOL          bFound      = FALSE; 
    HANDLE        hModuleSnap = NULL; 
    MODULEENTRY32 me32        = {0}; 
 
    // Take a snapshot of all modules in the specified process. 

    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID); 
    if (hModuleSnap == INVALID_HANDLE_VALUE) 
        return (FALSE); 
 
    // Fill the size of the structure before using it. 

    me32.dwSize = sizeof(MODULEENTRY32); 
 
    // Walk the module list of the process, and find the module of 
    // interest. Then copy the information to the buffer pointed 
    // to by lpMe32 so that it can be returned to the caller. 

    if (Module32First(hModuleSnap, &me32)) 
    { 
        do 
        { 
            if (me32.th32ModuleID == dwModuleID) 
            { 
                CopyMemory (lpMe32, &me32, cbMe32); 
                bFound = TRUE; 
            } 
        } 
        while (!bFound && Module32Next(hModuleSnap, &me32)); 
 
        bRet = bFound;   // if this sets bRet to FALSE, dwModuleID 
                         // no longer exists in specified process 
    } 
    else 
        bRet = FALSE;           // could not walk module list 
 
    // Do not forget to clean up the snapshot object. 

    CloseHandle (hModuleSnap); 
 
    return (bRet); 
} 

STDMETHODIMP Cprocess::get(BSTR name, LONG *dwProcessId)
{
    HANDLE         hProcessSnap = NULL; 
    BOOL           bRet      = FALSE; 
    PROCESSENTRY32 pe32      = {0}; 
 
    //  Take a snapshot of all processes in the system. 

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

    if (hProcessSnap == INVALID_HANDLE_VALUE) 
        return (FALSE); 
 
    //  Fill in the size of the structure before using it. 

    pe32.dwSize = sizeof(PROCESSENTRY32); 
 
    //  Walk the snapshot of the processes, and for each process, 
    //  display information. 
	_bstr_t aname(name);
	CString findName = (char *) aname, processName;
	findName.MakeLower();

    if (Process32First(hProcessSnap, &pe32)) 
    { 
        BOOL          bGotModule = FALSE; 
        MODULEENTRY32 me32       = {0}; 
 
        do 
        { 
			processName = pe32.szExeFile;
			processName.MakeLower();

			processName = processName.Right(processName.GetLength() - 
				processName.ReverseFind('\\') - 1);

            if( findName == processName) 
            { 
				*dwProcessId = pe32.th32ProcessID;
				return S_OK;
            } 
        } 
        while (Process32Next(hProcessSnap, &pe32)); 
        bRet = TRUE; 
    } 
    else 
        bRet = FALSE;    // could not walk the list of processes 
 
    // Do not forget to clean up the snapshot object. 

    CloseHandle (hProcessSnap); 

	*dwProcessId = 0;
	return S_OK;
}

STDMETHODIMP Cprocess::kill(LONG dwProcessId, LONG *result)
{
	HANDLE hProcess; 
 
	// Get the actual priority class. 
	hProcess = OpenProcess (PROCESS_TERMINATE, 
                    FALSE, dwProcessId); 

	TerminateProcess(hProcess, 0);

	return S_OK;
}

STDMETHODIMP Cprocess::getCurrentProcessId(LONG *pPID)
{
	*pPID = GetCurrentProcessId();

	return S_OK;
}

//
// загружает профиль владельца вызвавшей нити
//
STDMETHODIMP Cprocess::loadUserProfile()
{
	// создаем маркер доступа, при помощи которого будет
	// осуществлена загрузка профиля пользователя
	//
	BOOL bResult = ::OpenProcessToken(GetCurrentProcess(),
		TOKEN_ALL_ACCESS, &m_hToken);

	if(!bResult) return S_FALSE;

	TCHAR	lpszUserName[_MAX_PATH] = {0};
	DWORD	dwSize = sizeof(lpszUserName) - 1;

	::GetUserName(lpszUserName, &dwSize);

	m_profileInfo.dwSize = sizeof(PROFILEINFO);
	m_profileInfo.dwFlags = PI_NOUI;
	m_profileInfo.lpUserName = lpszUserName;

	bResult = ::LoadUserProfile(m_hToken, &m_profileInfo);

	if(!bResult) {
		CloseHandle(m_hToken);
		m_hToken = NULL;

		return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP Cprocess::unloadUserProfile()
{
	if(m_profileInfo.hProfile) 
	{
		// пытаемся выгрузить профиль пользователя
		//
		::UnloadUserProfile(m_hToken, m_profileInfo.hProfile);

		CloseHandle(m_hToken);
		
		m_profileInfo.hProfile = NULL;
		m_hToken = NULL;
	}

	return S_OK;
}

STDMETHODIMP Cprocess::isProcessAlive(LONG dwProcessId, BOOL *bResult)
{
	HANDLE hProcess; 
 
	// попытаемся открыть дескриптор процесса
	//
	hProcess = OpenProcess (PROCESS_QUERY_INFORMATION, FALSE, dwProcessId); 

	*bResult = hProcess == NULL ? 0 : 1;

	return S_OK;
}

STDMETHODIMP Cprocess::setProcessPriority(LONG lPriority)
{
	DWORD dwPriorityClass = 0;
	DWORD dwThreadPriority = 0;

	switch(lPriority)
	{
	case 1:
		dwPriorityClass = NORMAL_PRIORITY_CLASS;
		dwThreadPriority = THREAD_PRIORITY_NORMAL;
		break;
	case 2:
		dwPriorityClass = NORMAL_PRIORITY_CLASS;
		dwThreadPriority = THREAD_PRIORITY_BELOW_NORMAL;
		break;
	case 3:
		dwPriorityClass = IDLE_PRIORITY_CLASS;
		dwThreadPriority = THREAD_PRIORITY_LOWEST;
		break;
	}

	::SetPriorityClass(GetCurrentProcess(), dwPriorityClass);
	::SetThreadPriority(GetCurrentThread(), dwThreadPriority);

	return S_OK;
}
