// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__FAC3E80B_501F_451E_81F2_7371488CD715__INCLUDED_)
#define AFX_STDAFX_H__FAC3E80B_501F_451E_81F2_7371488CD715__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <afxwin.h>
#include <afxdisp.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include <comdef.h>

#include <afxext.h>         // MFC extensions
#include <afxtempl.h>			  // MFC template classes
#include <afxsock.h>        // MFC Sockets
#include <afxpriv.h>        // to get access to the T2A macro

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FAC3E80B_501F_451E_81F2_7371488CD715__INCLUDED)
