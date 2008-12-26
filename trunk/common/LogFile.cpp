// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <comdef.h>
#include <time.h>

#include "LogFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile() : m_hmodule(NULL), m_hfile(NULL)
{
}

CLogFile::~CLogFile()
{
	if(m_hfile)	CloseHandle(m_hfile);
}

bool CLogFile::Init(HMODULE hModule)
{
	m_hmodule = hModule;

	TCHAR lpszName[_MAX_PATH] = {0};

	if(!GetModuleFileName(hModule, lpszName, MAX_PATH - 1)) {
		strcpy(lpszName, "aseserver");
	}
	wsprintf(m_lpFileName, "%s.log", lpszName);

	m_hfile = CreateFile(m_lpFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);

	if(!m_hfile) return false;

	SetFilePointer(m_hfile, 0, NULL, FILE_END);
	return true;
}

void CLogFile::write(LPCTSTR what)
{
	if(!m_hfile || !what) return;

	TCHAR lpszBuffer[2048] = {0};
	TCHAR tmptime[128] = {0}, tmpdate[128] = {0};

    _strtime( tmptime ); _strdate( tmpdate );
    wsprintf( lpszBuffer, "%s %s -- %s\n", tmpdate, tmptime, what);

	DWORD dwWritten = 0;

	WriteFile(m_hfile, lpszBuffer, strlen(lpszBuffer), &dwWritten, NULL);

	FlushFileBuffers(m_hfile);
}
