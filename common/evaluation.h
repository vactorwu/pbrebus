// ScriptEvaluation.h: interface for the CScriptEvaluation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTEVALUATION_H__8BD8DDD7_3898_400F_98E1_05D4B954D481__INCLUDED_)
#define AFX_SCRIPTEVALUATION_H__8BD8DDD7_3898_400F_98E1_05D4B954D481__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>
#include <imagehlp.h>
#include "md5.h"

class CEvaluation
{
protected:
	char m_szProductSalt1[128],
		 m_szProductSalt2[128],
		 m_szClientInfo[128],
		 m_szProductKey[128],
		 m_szEvaluationCLSID[128];

public:
	CEvaluation() {};
	virtual ~CEvaluation() {};

	// set salt for product key generation
	void setProductSalt1(LPCTSTR lpszSalt) {
		strcpy( m_szProductSalt1, lpszSalt );
	}
	void setProductSalt2(LPCTSTR lpszSalt) {
		strcpy( m_szProductSalt2, lpszSalt );
	}
	// set client info and product key for passing registration
	void setClientInfo(LPCTSTR lpszClientInfo) {
		strcpy(m_szClientInfo, lpszClientInfo);
	}
	void setProductKey(LPCTSTR lpszProductKey) {
		strcpy(m_szProductKey, lpszProductKey);
	}
	// set CLSID of evaluation COM
	void setEvaluationCLSID(LPCTSTR lpszEvaluationCLSID) {
		strcpy(m_szEvaluationCLSID, lpszEvaluationCLSID);
	}

	// check for evaluation period
	virtual void passEvaluation( bool &bResult ) = 0;
	// check if product registered
	virtual void passRegistration( bool &bResult ) = 0;
	// setup initial product state for evaluation period
	virtual void setup() = 0;
	virtual bool was_setup() = 0;

	virtual bool canUse() = 0;
	virtual bool isRegistered() = 0;
	virtual bool isEvaluation() = 0;

	virtual DWORD getEvaluationTimeLeft() = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CEvaluation14Days : public CEvaluation
{
	DWORD	m_dwTimeKey;
	DWORD   m_dwInstanceCheckSum;
	DWORD   m_dwTimePeriod,
			m_dwEvaluationTimeLeft;
	int		*pState;

public:
	CEvaluation14Days() : CEvaluation(), m_dwEvaluationTimeLeft(0), 
		m_dwTimePeriod(238279948) // 1209600 (14)
	{
		// get current instance checksum
		char lpFileName[MAX_PATH] = {0};
		DWORD dwHeaderSum = 0, dwRealCheckSum = 0;

		GetModuleFileName(GetModuleHandle(NULL), lpFileName, MAX_PATH - 1);
		MapFileAndCheckSumA( lpFileName, &dwHeaderSum, &m_dwInstanceCheckSum);

		//
		m_dwTimeKey = 0xA6B21793;

		pState = new int[3];
		memset(pState, 0, 3 * sizeof(int));

		m_dwTimePeriod = m_dwTimePeriod ^ 0x0e21a80c;
	};

	virtual ~CEvaluation14Days() 
	{
		delete[] pState;
	};

	virtual void resetState()
	{
		bool bResult;
		m_dwEvaluationTimeLeft = 0;
		pState[0] = pState[1] = pState[2] = 0;
		
		// the product is registered
		passRegistration( bResult );
		pState[2] = bResult;

		if(!pState[2]) {
			// the evaluation period continues
			passEvaluation(bResult);
			pState[1] = bResult;
		}
		
		// user can use the product;
		pState[0] = pState[1] || pState[2];
	}

	virtual bool canUse() {
		return pState[0] == 1;
	}

	virtual bool isEvaluation() {
		return pState[1] == 1;
	}

	virtual bool isRegistered() {
		return pState[2] == 1;
	}

	virtual DWORD getEvaluationTimeLeft()
	{
		return (DWORD) m_dwEvaluationTimeLeft / (60 * 60 * 24);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void passEvaluation( bool &bResult )
	{
		time_t currentTime; time(&currentTime);
		time_t setupTime = 0;
		double dwCurrentDifference;

		bResult = false;

		HKEY hkey;
		bool bKeyExists = false;

		// get creation time of COM-object
		char szComPath[1024] = {0};
		sprintf( szComPath, "CLSID\\%s\\%s", m_szEvaluationCLSID, "InprocServer32");

		if(RegOpenKeyExA(HKEY_CLASSES_ROOT, szComPath, 0,
			KEY_QUERY_VALUE, &hkey) == ERROR_SUCCESS) {

			char szModuleName[1024] = {0};
			DWORD dwSize = sizeof(szModuleName);
			DWORD dwType = 0;

			if(RegQueryValueExA( hkey, NULL, NULL, 
				&dwType, (LPBYTE) szModuleName, &dwSize) == ERROR_SUCCESS) 
			{
				FILETIME ftCreate, ftAccess, ftWrite;
				SYSTEMTIME stUTC, stLocal;

				HANDLE hFile = CreateFileA( szModuleName, FILE_READ_EA, FILE_SHARE_READ,
					NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				// retrieve the file times for the file.
				if (hFile && GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
				{
					// convert the last-write time to local time.
					FileTimeToSystemTime(&ftCreate, &stUTC);
					SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

					tm tmCreate = {stLocal.wSecond, stLocal.wMinute, stLocal.wHour, stLocal.wDay, 
						stLocal.wMonth - 1, stLocal.wYear - 1900, stLocal.wDayOfWeek,
						0, 0};

					time_t tCreate = mktime( &tmCreate );
					dwCurrentDifference = difftime(currentTime, tCreate);
					m_dwEvaluationTimeLeft = __max(m_dwTimePeriod - dwCurrentDifference, 0);

					if(dwCurrentDifference > m_dwTimePeriod) 
					{
						RegCloseKey(hkey);
						if(hFile) CloseHandle(hFile);

						return; // 14 days
					}

					bKeyExists = true; 
					bResult = true;
				}
				if(hFile) CloseHandle(hFile);
			}

			if(!bKeyExists) {
				DWORD dwReserved = 0, dwDataSize = sizeof(setupTime);
				dwType = 0;

				if(RegQueryValueExA( hkey, "Configuration", NULL, 
					&dwType, (LPBYTE) &setupTime, &dwDataSize) == ERROR_SUCCESS) 
				{
					// decode time
					setupTime = setupTime ^ m_dwTimeKey;
					bKeyExists = true;
				}

				// compare setup time and current one
				dwCurrentDifference = difftime(currentTime, setupTime);
				m_dwEvaluationTimeLeft = __max(m_dwTimePeriod - dwCurrentDifference, 0);

				bResult = difftime(currentTime, setupTime) < m_dwTimePeriod; // 14 days;
			}			

			RegCloseKey(hkey);
		}
	}	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void passRegistration( bool &bResult )
	{
		bResult = false;

		m_dwInstanceCheckSum = 0;

		// compare stored product key with calculated product key
		char szCalcProductKey[1024] = {0};
		sprintf(szCalcProductKey, "%s%d%s%s", m_szProductSalt1, m_dwInstanceCheckSum, 
			m_szProductSalt2, m_szClientInfo);
			
		if(strcmp( MD5String( szCalcProductKey ), m_szProductKey) == 0) bResult = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual bool was_setup()
	{
		char szComPath[1024] = {0};
		sprintf( szComPath, "CLSID\\%s\\%s", m_szEvaluationCLSID, "InprocServer32");

		HKEY hkey;
		if(RegOpenKeyExA(HKEY_CLASSES_ROOT, szComPath, 0,
			KEY_ALL_ACCESS, &hkey) != ERROR_SUCCESS) 
		{
			return false;
		}

		RegCloseKey(hkey);
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void setup()
	{
		// get current time
		time_t currentTime; time(&currentTime);

		char szComPath[1024] = {0};
		sprintf( szComPath, "CLSID\\%s\\%s", m_szEvaluationCLSID, "InprocServer32");

		HKEY hkey;
		if(RegOpenKeyExA(HKEY_CLASSES_ROOT, szComPath, 0,
			KEY_ALL_ACCESS, &hkey) != ERROR_SUCCESS) return;

		DWORD dwDataSize = sizeof(currentTime);
		DWORD dwType = REG_DWORD, dwReserved = 0;

		// encode time
		currentTime = currentTime ^ m_dwTimeKey;

		RegSetValueExA(hkey, "Configuration", dwReserved, 
			dwType, (LPBYTE) &currentTime, dwDataSize);

		char szModuleName[1024] = {0};
		DWORD dwSize = sizeof(szModuleName);
		dwType = 0;

		if(RegQueryValueExA( hkey, NULL, NULL, 
			&dwType, (LPBYTE) szModuleName, &dwSize) == ERROR_SUCCESS) 
		{
			FILETIME ftCreate;
			SYSTEMTIME stLocal;

			HANDLE hFile = CreateFile( szModuleName, FILE_WRITE_ATTRIBUTES, FILE_SHARE_WRITE,
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile) 
			{
				GetSystemTime(&stLocal);
				SystemTimeToFileTime(&stLocal, &ftCreate);

				SetFileTime(hFile, &ftCreate, NULL, NULL);
				CloseHandle(hFile);
			}
		}

		RegCloseKey(hkey);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCEvaluation14DaysExt : public CEvaluation14Days
{
public:
	CCEvaluation14DaysExt() : CEvaluation14Days() {};
	virtual ~CCEvaluation14DaysExt() {};

	virtual void passEvaluation( bool &bResult )
	{
		CEvaluation14Days::passEvaluation( bResult );
		if(!bResult) return;
	}	
	virtual void passRegistration( bool &bResult )
	{
		CEvaluation14Days::passRegistration( bResult );
		if(!bResult) return;
	}
};

#endif // !defined(AFX_SCRIPTEVALUATION_H__8BD8DDD7_3898_400F_98E1_05D4B954D481__INCLUDED_)
