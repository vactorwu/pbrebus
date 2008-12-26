// EventLog.h: interface for the CEventLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTLOG_H__10237160_12C9_4A88_903B_1ACC750BC47C__INCLUDED_)
#define AFX_EVENTLOG_H__10237160_12C9_4A88_903B_1ACC750BC47C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEventLog  
{
public:
	CEventLog() {;}
	virtual ~CEventLog() {;}

	void write(LPCTSTR lpszMessage)
	{
	}

};

#endif // !defined(AFX_EVENTLOG_H__10237160_12C9_4A88_903B_1ACC750BC47C__INCLUDED_)
