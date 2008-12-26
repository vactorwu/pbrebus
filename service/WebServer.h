// WebServer.h: interface for the CWebServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBSERVER_H__046A9993_39BE_443A_9EE5_9544D207E55A__INCLUDED_)
#define AFX_WEBSERVER_H__046A9993_39BE_443A_9EE5_9544D207E55A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class Log = CLogFile>
class CWebServer  
{
	int m_sock;

public:
	Log &m_log;

public:
	CWebServer(Log &log);
	virtual ~CWebServer(void);

	void stop();
	void init();
	void loop();

};

#endif // !defined(AFX_WEBSERVER_H__046A9993_39BE_443A_9EE5_9544D207E55A__INCLUDED_)
