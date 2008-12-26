// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <comdef.h>
#include <time.h>

#include <string>
#include "LogFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile() : m_hmodule(NULL), m_hfile(NULL), debuglevel_(1)
{
}

CLogFile::~CLogFile()
{
	if(m_hfile)	CloseHandle(m_hfile);
}

bool CLogFile::Init(HMODULE hModule, int debuglevel)
{
	debuglevel_ = debuglevel;
	m_hmodule = hModule;

	TCHAR lpszName[_MAX_PATH] = {0};

	if(!GetModuleFileName(hModule, lpszName, MAX_PATH - 1)) {
		strcpy(lpszName, _T("aseserver"));
	}
	wsprintf(m_lpFileName, "%s.log", lpszName);

	m_hfile = CreateFile(m_lpFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(!m_hfile) return false;

	DWORD dwFileSize = GetFileSize( m_hfile, NULL );
	if(dwFileSize > 524288) 
	{
		CloseHandle(m_hfile);
		
		m_hfile = CreateFile(m_lpFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if(!m_hfile) return false;
	}

	SetFilePointer(m_hfile, 0, NULL, FILE_END);
	return true;
}

void CLogFile::write(LPCTSTR what, int debuglevel)
{
	if(!m_hfile || !what || debuglevel > debuglevel_) return;

	TCHAR lpszBuffer[65535] = {0};
	TCHAR tmptime[128] = {0}, tmpdate[128] = {0};

    _strtime( tmptime ); _strdate( tmpdate );
    wsprintf( lpszBuffer, "%s %s -- %s\n", tmpdate, tmptime, what);

	DWORD dwWritten = 0;

	WriteFile(m_hfile, lpszBuffer, strlen(lpszBuffer), &dwWritten, NULL);

	FlushFileBuffers(m_hfile);
}

void CLogFile::write(LPCTSTR what, _com_error &e, int debuglevel)
{
	char buffer[255] = {0};
	std::string text(what);

	text += ", err code: ";
	text += itoa(e.Error(), buffer, 16);

	IErrorInfo *p_errinf = e.ErrorInfo();
	if(p_errinf) 
	{
		if(e.Description().length() > 0) {
			text += ", err desc: ";
			text += (char *) e.Description();
		}

		if(e.Source().length() > 0) {
			text += ", err source: ";
			text += (char *) e.Source();
		}
		p_errinf->Release();
	}
	else
	{
		text += ", err desc: ";
		text += e.ErrorMessage();
	}

	write(text.c_str(), debuglevel);
}
