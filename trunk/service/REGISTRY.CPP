//////////////////////////////////////////////////////////////////
// Registry.cpp
// A simple registry class
// Julian Smart

#include "stdafx.h"
#include "Registry.h"
#include "winreg.h"

// RegisterApp:
// HKEY_CLASSES_ROOT\.ext = appId
// HKEY_CLASSES_ROOT\appId = typeName
// HKEY_CLASSES_ROOT\appId\DefaultIcon = defaultIcon
// where typeName is the human-readable form of appId.	
//
// If contentType is specified, the MIME type is stored:
// HKEY_CLASSES_ROOT\.ext\Content Type = contentType
//
// If you don't have an extension to register, then
// you don't need to call RegisterApp.

BOOL CRegistry::RegisterApp(const CString& ext, const CString& appId,
		const CString& typeName, const CString& defaultIcon, const CString& contentType)
{
/*
	HKEY hKey = 0;
	long ret = ::RegOpenKey(HKEY_CLASSES_ROOT, (const char *) ext, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		ret = ::RegCeateKey(HKEY_CLASSES_ROOT, (const char *) ext, &hKey);
		if (ret != ERROR_SUCCESS)
			return FALSE;
	}
*/

	long ret = ::RegSetValue(HKEY_CLASSES_ROOT, (const char *) ext, REG_SZ,
		(const char *) typeName, typeName.GetLength());
	if (ret != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CRegistry::HaveRegistered(const CString& appId) const
{
	HKEY hKey = 0;
	long ret = ::RegOpenKeyEx(HKEY_CLASSES_ROOT, (const char *) appId,
		0, KEY_READ, & hKey);

	if (ret == ERROR_SUCCESS)
	{
		::RegCloseKey(hKey);
		return TRUE;
	}
	else
		return FALSE;
}

// Get the name corresponding to this extension, or FALSE
BOOL CRegistry::GetExtension(const CString& ext, CString& appName)
{
	char buf[256];
	long sz = 0;
	long ret = ::RegQueryValue(HKEY_CLASSES_ROOT, (const char *)ext,
		buf, &sz);
	if (ret == ERROR_SUCCESS)
	{
		appName = buf;
		return TRUE;
	}
	else
		return FALSE;
}

// Determine how the app will be opened
// HKEY_CLASSES_ROOT\appId\shell\open\command = openCommand
// HKEY_CLASSES_ROOT\appId\shell\open\ddeexec = ddeCommand
BOOL CRegistry::RegisterShellOpen(const CString& appId, const CString& openCommand,
		const CString& ddeCommand)
{
	return TRUE;
}

// Register a sound event (user must then associate sound using Control
// Panel)
// HKEY_CURRENT_USER\AppEvents\Event Labels\eventName = readableEventName
BOOL CRegistry::RegisterSound(const CString& eventName, const CString& readableEventName)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////
// GENERIC FUNCTIONS

// System-wide stuff, e.g. home directory
// HKEY_CLASSES_ROOT\appId\key = value
BOOL CRegistry::SetSystemEntry(const CString& appId, const CString& key, const CString& value)
{
	CString key1(appId);
	key1 += CString("\\") + key;

	return SetEntry(CREGISTRY_SYSTEM, key1, value);
}

BOOL CRegistry::SetSystemEntry(const CString& appId, const CString& key, const long value)
{
	CString key1(appId);
	key1 += CString("\\") + key;

	return SetEntry(CREGISTRY_SYSTEM, key1, value);
}

// Does the key exist?
BOOL CRegistry::HasSystemKey(const CString& appId, const CString& key)
{
	CString key1(appId);
	key1 += CString("\\") + key;

	return HasKey(CREGISTRY_SYSTEM, key1);
}

// Get the entry
BOOL CRegistry::GetSystemEntry(const CString& appId, const CString& key, CString& value)
{
	CString key1(appId);
	key1 += CString("\\") + key;

	return GetEntry(CREGISTRY_SYSTEM, key1, value);
}

BOOL CRegistry::GetSystemEntry(const CString& appId, const CString& key, long& value)
{
	CString key1(appId);
	key1 += CString("\\") + key;

	return GetEntry(CREGISTRY_SYSTEM, key1, value);
}

// User-specific data, e.g. user's current home directory
// HKEY_CURRENT_USER\SOFTWARE\appId\key = value
BOOL CRegistry::SetUserEntry(const CString& appId, const CString& key, const CString& value)
{
	CString key1("SOFTWARE\\");
	if (m_organisation != "")
		key1 += m_organisation + CString("\\") ;
	key1 += appId + CString("\\") + key;

	return SetEntry(CREGISTRY_USER, key1, value);
}

BOOL CRegistry::SetUserEntry(const CString& appId, const CString& key, const long value)
{
	CString key1("SOFTWARE\\");
	if (m_organisation != "")
		key1 += m_organisation + CString("\\") ;
	key1 += appId + CString("\\") + key;

	return SetEntry(CREGISTRY_USER, key1, value);
}

// Does the key exist?
BOOL CRegistry::HasUserKey(const CString& appId, const CString& key)
{
	CString key1("SOFTWARE\\");
	if (m_organisation != "")
		key1 += m_organisation + CString("\\") ;
	key1 += appId + CString("\\") + key;

	return HasKey(CREGISTRY_USER, key1);
}

// Get the entry
BOOL CRegistry::GetUserEntry(const CString& appId, const CString& key, CString& value)
{
	CString key1("SOFTWARE\\");
	if (m_organisation != "")
		key1 += m_organisation + CString("\\") ;
	key1 += appId + CString("\\") + key;

	return GetEntry(CREGISTRY_USER, key1, value);
}

BOOL CRegistry::GetUserEntry(const CString& appId, const CString& key, long& value)
{
	CString key1("SOFTWARE\\");
	if (m_organisation != "")
		key1 += m_organisation + CString("\\") ;
	key1 += appId + CString("\\") + key;

	return GetEntry(CREGISTRY_USER, key1, value);
}

// Machine-specific data, such as data common to all users.
// HKEY_LOCAL_MACHINE\SOFTWARE\appId\key = value
BOOL CRegistry::SetMachineEntry(const CString& appId, const CString& key, const CString& value)
{
	CString key1("SOFTWARE\\");
	if (m_organisation != "")
		key1 += m_organisation + CString("\\") ;
	key1 += appId + CString("\\") + key;

	return SetEntry(CREGISTRY_MACHINE, key1, value);
}

BOOL CRegistry::SetMachineEntry(const CString& appId, const CString& key, const long value)
{
	CString key1("SOFTWARE\\");
	if (m_organisation != "")
		key1 += m_organisation + CString("\\") ;
	key1 += appId + CString("\\") + key;

	return SetEntry(CREGISTRY_MACHINE, key1, value);
}

// Does the key exist?
BOOL CRegistry::HasMachineKey(const CString& appId, const CString& key)
{
	CString key1("SOFTWARE\\");
	if (m_organisation != "")
		key1 += m_organisation + CString("\\") ;
	key1 += appId + CString("\\") + key;

	return HasKey(CREGISTRY_MACHINE, key1);
}

// Get the entry
BOOL CRegistry::GetMachineEntry(const CString& appId, const CString& key, CString& value)
{
	CString key1("SOFTWARE\\");
	if (m_organisation != "")
		key1 += m_organisation + CString("\\") ;
	key1 += appId + CString("\\") + key;

	return GetEntry(CREGISTRY_MACHINE, key1, value);
}

BOOL CRegistry::GetMachineEntry(const CString& appId, const CString& key, long& value)
{
	CString key1("SOFTWARE\\");
	if (m_organisation != "")
		key1 += m_organisation + CString("\\") ;
	key1 += appId + CString("\\") + key;

	return GetEntry(CREGISTRY_MACHINE, key1, value);
}

//////////////////////////////////////////////////////////////////
// GENERAL REGISTRY FUNCTIONS
BOOL CRegistry::SetEntry(const int majorKey, const CString& subKey, const CString& value)
{
	HKEY hMajorKey = 0;
	switch (majorKey)
	{
		case CREGISTRY_SYSTEM:
			{
				hMajorKey = HKEY_CLASSES_ROOT;
				break;
			}
		case CREGISTRY_USER:
			{
				hMajorKey = HKEY_CURRENT_USER;
				break;
			}
		case CREGISTRY_MACHINE:
			{
				hMajorKey = HKEY_LOCAL_MACHINE;
				break;
			}
		default:
			return FALSE;

	}

	CString actualKey, name;
	SplitKeyName(subKey, actualKey, name);

	HKEY hKey = 0;
	long ret = ::RegOpenKeyEx(hMajorKey, (const char *) actualKey,
		0, KEY_ALL_ACCESS, & hKey);

	if (ret != ERROR_SUCCESS)
	{
		// Create key
		char classType[1];
		classType[0] = 0;
		DWORD disposition = 0;
		ret = ::RegCreateKeyEx(hMajorKey, (const char *) actualKey,
			0, classType, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
			& hKey, & disposition);

		if (ret != ERROR_SUCCESS)
			return FALSE;
	}

	DWORD reserved = 0;
	DWORD type = REG_SZ;
	BYTE *buf = new BYTE[value.GetLength() + 1];
	strcpy((char *)buf, (const char *) value);
	DWORD sz = value.GetLength() + 1;
	ret = ::RegSetValueEx(hKey, (const char *) name, reserved, 
		type, buf, sz);
	delete[] buf;
	
	::RegCloseKey(hKey);
	
	if (ret == ERROR_SUCCESS)
	{
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CRegistry::SetEntry(const int majorKey, const CString& subKey, const long value)
{
	CString valueStr;
	valueStr.Format("%ld", value);
	return SetEntry(majorKey, subKey, valueStr);
}

// Does the key exist?
BOOL CRegistry::HasKey(const int majorKey, const CString& subKey)
{
	HKEY hMajorKey = 0;
	switch (majorKey)
	{
		case CREGISTRY_SYSTEM:
			{
				hMajorKey = HKEY_CLASSES_ROOT;
				break;
			}
		case CREGISTRY_USER:
			{
				hMajorKey = HKEY_CURRENT_USER;
				break;
			}
		case CREGISTRY_MACHINE:
			{
				hMajorKey = HKEY_LOCAL_MACHINE;
				break;
			}
		default:
			return FALSE;

	}
	CString actualKey, name;
	SplitKeyName(subKey, actualKey, name);

	HKEY hKey = 0;
	long ret = ::RegOpenKeyEx(hMajorKey, (const char *) actualKey,
		0, KEY_READ, & hKey);

	if (ret == ERROR_SUCCESS)
	{
		::RegCloseKey(hKey);
		return TRUE;
	}
	else
		return FALSE;
}

// Get the entry
BOOL CRegistry::GetEntry(const int majorKey, const CString& subKey, CString& value)
{
	HKEY hMajorKey = 0;
	switch (majorKey)
	{
		case CREGISTRY_SYSTEM:
			{
				hMajorKey = HKEY_CLASSES_ROOT;
				break;
			}
		case CREGISTRY_USER:
			{
				hMajorKey = HKEY_CURRENT_USER;
				break;
			}
		case CREGISTRY_MACHINE:
			{
				hMajorKey = HKEY_LOCAL_MACHINE;
				break;
			}
		default:
			return FALSE;

	}

	CString actualKey, name;
	SplitKeyName(subKey, actualKey, name);

	HKEY hKey = 0;
	long ret = ::RegOpenKeyEx(hMajorKey, (const char *) actualKey,
		0, KEY_READ, & hKey);

	if (ret == ERROR_SUCCESS)
	{
		DWORD reserved = 0;
		DWORD type = 0;
		BYTE buf[256] = "\x0";
		DWORD sz = 256;
		ret = ::RegQueryValueEx(hKey, (const char *) name, NULL, 
			&type, buf, & sz);
		::RegCloseKey(hKey);

		if (ret == ERROR_SUCCESS)
		{
			return ConvertToString(buf, type, value);
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}

BOOL CRegistry::GetEntry(const int majorKey, const CString& subKey, long& value)
{
	CString str;
	if (GetEntry(majorKey, subKey, str))
	{
		value = atol((const char *)str);
		return TRUE;
	}
	else
		return FALSE;
}

// KEY\Name -> Key, Name
// Name -> "", Name
BOOL CRegistry::SplitKeyName(const CString& keyName, CString& key, CString& name)
{
	int i = keyName.GetLength() - 1;
	while (i >= 0)
	{
		if (keyName[i] == '\\')
		{
			key = keyName.Left(i);
			name = keyName.Right(keyName.GetLength() - i - 1);
			return TRUE;
		}
		i --;
	}
	key = "";
	name = keyName;
	return TRUE;
}

// Convert whatever type it is, to a string.
BOOL CRegistry::ConvertToString(const BYTE buf[], const DWORD type, CString& str)
{
	switch (type)
	{
	case REG_DWORD:
		{
			char buf2[20];
			sprintf(buf2, "%ld", (DWORD) buf);
			str = buf2;
			return TRUE;
		}
	case REG_SZ:
	case REG_EXPAND_SZ:
		{
			str = buf;
			return TRUE;
		}
	case REG_BINARY:
		{
			str = "Binary data"; // Can't deal with it
			return FALSE;
		}
	default:
		{
			str = "Can't convert to string";
			return FALSE;
		}
	}
	return FALSE;
}

void CRegistry::DeleteUserEntry(const CString &appid, const CString &key)
{
	CString key1("SOFTWARE\\");
	if (m_organisation != "")
		key1 += m_organisation + CString("\\") ;
	key1 += appid + CString("\\") + key;

	CString actualKey, name;
	SplitKeyName(key1, actualKey, name);

	HKEY hKey = 0;
	long ret = ::RegOpenKeyEx(HKEY_CURRENT_USER, (const char *) actualKey,
		0, KEY_ALL_ACCESS, & hKey);

	if (ret == ERROR_SUCCESS)
	{
		::RegDeleteValue(hKey, (const char *) name);
	}
}
