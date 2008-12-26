// csystem.cpp : Implementation of CSystem
#include "stdafx.h"
#include "Syslevel.h"
#include "csystem.h"

#include <comdef.h>
/////////////////////////////////////////////////////////////////////////////
// CSystem


STDMETHODIMP CSystem::getComputerName(BSTR *pName)
{
	TCHAR lpszName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwLength = MAX_COMPUTERNAME_LENGTH;
	_bstr_t name;

	BOOL bResult = GetComputerName(lpszName, &dwLength);

	if(!bResult) 
	{
		LPVOID lpMsgBuf;
		if (!FormatMessage( 
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
		   return S_FALSE;
		}

		name = (LPTSTR) lpMsgBuf;

		// Free the buffer.
		LocalFree( lpMsgBuf );
	}
	else
		name = lpszName;

	*pName = name.copy();
	
	return S_OK;
}
