//////////////////////////////////////////////////////////////////
// Registry.h
// A simple registry class
// Julian Smart

#ifndef __REGISTRYH__
#define __REGISTRYH__

#define CREGISTRY_SYSTEM	1
#define CREGISTRY_USER		2
#define CREGISTRY_MACHINE	3

class CRegistry
{
public:
	void DeleteUserEntry(const CString &appid, const CString &key);
	CRegistry(const CString& org = "") { m_organisation = org; }

	//////////////////////////////////////////////////////////////////
	// HIGH-LEVEL FUNCTIONS

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

	BOOL RegisterApp(const CString& ext, const CString& appId,
		const CString& typeName, const CString& defaultIcon = "", const CString& contentType = "");

	BOOL HaveRegistered(const CString& appId) const;

	// Determine how the app will be opened
	// HKEY_CLASSES_ROOT\appId\shell\open\command = openCommand
	// HKEY_CLASSES_ROOT\appId\shell\open\ddeexec = ddeCommand
	BOOL RegisterShellOpen(const CString& appId, const CString& openCommand,
		const CString& ddeCommand = "");

	// Register a sound event (user must then associate sound using Control
	// Panel)
	// HKEY_CURRENT_USER\AppEvents\Event Labels\eventName = readableEventName
	BOOL RegisterSound(const CString& eventName, const CString& readableEventName);

	// Get the name corresponding to this extension, or FALSE
	BOOL GetExtension(const CString& ext, CString& appName);

	//////////////////////////////////////////////////////////////////
	// GENERIC FUNCTIONS

	// System-wide stuff, e.g. home directory
	// HKEY_CLASSES_ROOT\appId\key = value
	BOOL SetSystemEntry(const CString& appId, const CString& key, const CString& value);
	BOOL SetSystemEntry(const CString& appId, const CString& key, const long value);

	// Does the key exist?
	BOOL HasSystemKey(const CString& appId, const CString& key);

	// Get the entry
	BOOL GetSystemEntry(const CString& appId, const CString& key, CString& value);
	BOOL GetSystemEntry(const CString& appId, const CString& key, long& value);

	// User-specific data, e.g. user's current home directory
	// HKEY_CURRENT_USER\SOFTWARE\appId\key = value
	BOOL SetUserEntry(const CString& appId, const CString& key, const CString& value);
	BOOL SetUserEntry(const CString& appId, const CString& key, const long value);

	// Does the key exist?
	BOOL HasUserKey(const CString& appId, const CString& key);

	// Get the entry
	BOOL GetUserEntry(const CString& appId, const CString& key, CString& value);
	BOOL GetUserEntry(const CString& appId, const CString& key, long& value);

	// Machine-specific data, such as data common to all users.
	// HKEY_LOCAL_MACHINE\SOFTWARE\appId\key = value
	BOOL SetMachineEntry(const CString& appId, const CString& key, const CString& value);
	BOOL SetMachineEntry(const CString& appId, const CString& key, const long value);

	// Does the key exist?
	BOOL HasMachineKey(const CString& appId, const CString& key);

	// Get the entry
	BOOL GetMachineEntry(const CString& appId, const CString& key, CString& value);
	BOOL GetMachineEntry(const CString& appId, const CString& key, long& value);

	//////////////////////////////////////////////////////////////////
	// GENERAL REGISTRY FUNCTIONS
	BOOL SetEntry(const int majorKey, const CString& subKey, const CString& value);
	BOOL SetEntry(const int majorKey, const CString& subKey, const long value);

	// Does the key exist?
	BOOL HasKey(const int majorKey, const CString& subKey);

	// Get the entry
	BOOL GetEntry(const int majorKey, const CString& subKey, CString& value);
	BOOL GetEntry(const int majorKey, const CString& subKey, long& value);

	//////////////////////////////////////////////////////////////////
	// GENERAL REGISTRY FUNCTIONS
	inline void SetOrganisation(const CString& str) { m_organisation = str; }
	inline CString GetOrganisation(void) { return m_organisation; }
	
	//////////////////////////////////////////////////////////////////
	// UTILITIES
	static BOOL SplitKeyName(const CString& keyName, CString& key, CString& name);
	static BOOL ConvertToString(const BYTE buf[], const DWORD type, CString& str);

protected:
	CString		m_organisation;
};



#endif
