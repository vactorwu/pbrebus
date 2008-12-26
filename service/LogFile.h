// LogFile.h: interface for the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILE_H__4DC84AB8_1FEA_48F7_B249_1EA65AE477BA__INCLUDED_)
#define AFX_LOGFILE_H__4DC84AB8_1FEA_48F7_B249_1EA65AE477BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLogFile
{
	HANDLE m_hfile;
	HMODULE m_hmodule;
	TCHAR m_lpFileName[MAX_PATH];

	int debuglevel_;

public:
	virtual void write(LPCTSTR what, int debuglevel = 1);
	virtual void write(LPCTSTR what, _com_error &e, int debuglevel = 1);

	CLogFile();
	virtual ~CLogFile();

	bool Init(HMODULE hModule, int debuglevel = 1);
};

#endif // !defined(AFX_LOGFILE_H__4DC84AB8_1FEA_48F7_B249_1EA65AE477BA__INCLUDED_)
