// ORCASession.cpp: implementation of the CORCASession class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ORCASession.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CORCASession::CORCASession() : m_hSession(NULL), m_hLibrary(NULL),
	m_version(NONE)
{
	memset(lpFunction, 0, sizeof(LPVOID) * FUNCTIONS_NUM);
}

CORCASession::~CORCASession()
{
	try {
		if(m_hSession) {
			SessionClose();
			m_hSession = NULL;
		}
	}
	catch(...) {
	}

	if(m_hLibrary) {
		FreeLibrary(m_hLibrary);
		m_hLibrary = NULL;
	}
}

LPCSTR lpszfunctions[FUNCTIONS_NUM] = {
	"PBORCA_SessionClose",
	"PBORCA_SessionGetError",
	"PBORCA_SessionOpen",
	"PBORCA_SessionSetCurrentAppl",
	"PBORCA_SessionSetLibraryList",
	"PBORCA_LibraryCommentModify",
	"PBORCA_LibraryCreate",
	"PBORCA_LibraryDelete",
	"PBORCA_LibraryDirectory",
	"PBORCA_LibraryEntryCopy",
	"PBORCA_LibraryEntryDelete",
	"PBORCA_LibraryEntryExport",
	"PBORCA_LibraryEntryInformation",
	"PBORCA_LibraryEntryMove",
//	"PBORCA_CheckOutEntry",
//	"PBORCA_CheckInEntry",
//	"PBORCA_ListCheckOutEntries",
	"",
	"",
	"",
	"PBORCA_CompileEntryImport",
	"PBORCA_CompileEntryImportList",
	"PBORCA_CompileEntryRegenerate",
	"PBORCA_ApplicationRebuild",
	"PBORCA_ExecutableCreate",
	"PBORCA_DynamicLibraryCreate",
	"PBORCA_ObjectQueryHierarchy",
	"PBORCA_ObjectQueryReference" };

bool CORCASession::Use(LPCSTR lpszVersion)
{
	if(strcmp(lpszVersion, "PowerBuilder 8.0") == 0) {
		return orca.Use(CORCASession::PB_8);
	} 
	else if(strcmp(lpszVersion, "PowerBuilder 9.0") == 0)
	{
		return orca.Use(CORCASession::PB_9);
	}
	else
	{
		return orca.Use(CORCASession::PB_6);
	}
}

bool CORCASession::Use(enumVersion ver)
{
	if(m_hSession) SessionClose();
	if(m_hLibrary) FreeLibrary(m_hLibrary);

	memset(lpFunction, 0, sizeof(LPVOID) * FUNCTIONS_NUM);

	m_hSession = m_hLibrary = NULL;

	LPCSTR lpszLibrary;

	switch(ver) 
	{
	case PB_6:
		lpszLibrary = "pborc60.dll";
		break;
	case PB_8:
		lpszLibrary = "pborc80.dll";
		break;
	case PB_9:
		lpszLibrary = "pborc90.dll";
		break;
	}

	m_hLibrary = LoadLibrary(lpszLibrary);
	if(!m_hLibrary) {
		CString msg; msg.Format("LoadLibrary failed: %s", lpszLibrary);
		log.write(msg);

		return false;
	}

	CString msg;

	for(int i = 0; i < FUNCTIONS_NUM; i++) {

		if(strcmp(lpszfunctions[i], "") == 0) continue;

		lpFunction[i] = (LPVOID) GetProcAddress(m_hLibrary, 
			lpszfunctions[i]);

		if(!lpFunction[i]) {
			msg.Format("loading function failed: %s", lpszfunctions[i]);
			log.write(msg);

			return false;
		}
	}

	m_version = ver;

	return true;
}

PBWINAPI_(VOID) CORCASession::SessionClose ()
{
	if(!m_hSession) return;

	((void (WINAPI *) (HPBORCA)) lpFunction[0])(m_hSession);

	m_hSession = NULL;
}

PBWINAPI_(VOID) CORCASession::SessionGetError (LPTSTR   lpszErrorBuffer,
												INT iErrorBufferSize )
{
	if(!m_hSession) return;

	((void (WINAPI *) (HPBORCA, LPTSTR, INT)) lpFunction[1])
		(m_hSession, lpszErrorBuffer, iErrorBufferSize);
}

bool CORCASession::SessionUse(HPBORCA hSession)
{
	if(!m_hLibrary || !hSession) return false;

	m_hSession = hSession;
	return true;
}

PBWINAPI_(VOID) CORCASession::SessionOpen( void )
{
	if(!m_hLibrary) return;

	m_hSession = ((HPBORCA (WINAPI *) (void)) lpFunction[2])();

	return;
}

PBWINAPI_(INT) CORCASession::SessionSetCurrentAppl (LPTSTR   lpszApplLibName,
													LPTSTR   lpszApplName )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR)) lpFunction[3])
		(m_hSession, lpszApplLibName, lpszApplName);
}

PBWINAPI_(INT) CORCASession::SessionSetLibraryList (LPTSTR FAR *pLibNames, 
													INT iNumberOfLibs )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR FAR *, INT)) lpFunction[4])
		(m_hSession, pLibNames, iNumberOfLibs);
}

PBWINAPI_(INT) CORCASession::LibraryCommentModify (LPTSTR   lpszLibName,  
                                             LPTSTR   lpszLibComments )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR)) lpFunction[5])
		(m_hSession, lpszLibName, lpszLibComments);
}

PBWINAPI_(INT) CORCASession::LibraryCreate(LPTSTR   lpszLibName,  
                                         LPTSTR   lpszLibComments )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR)) lpFunction[6])
		(m_hSession, lpszLibName, lpszLibComments);
}

PBWINAPI_(INT) CORCASession::LibraryDelete (LPTSTR   lpszLibName )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR)) lpFunction[7])
		(m_hSession, lpszLibName);
}

PBWINAPI_(INT) CORCASession::LibraryDirectory (LPTSTR           lpszLibName,       
                                         LPTSTR           lpszLibComments,  
                                         INT             iCmntsBuffSize, 
                                         PBORCA_LISTPROC pListProc, 
                                         LPVOID          pUserData )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, 
			INT, PBORCA_LISTPROC, LPVOID)) lpFunction[8])
		(m_hSession, lpszLibName, lpszLibComments, iCmntsBuffSize,
			pListProc, pUserData);
}

PBWINAPI_(INT) CORCASession::LibraryEntryCopy (LPTSTR       lpszSourceLibName,  
                                         LPTSTR       lpszDestLibName,  
                                         LPTSTR       lpszEntryName, 
                                         PBORCA_TYPE otEntryType )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, 
			LPTSTR, PBORCA_TYPE)) lpFunction[9])
		(m_hSession, lpszSourceLibName, lpszDestLibName, lpszEntryName,
			otEntryType);
}

PBWINAPI_(INT) CORCASession::LibraryEntryDelete (LPTSTR       lpszLibName,  
                                           LPTSTR       lpszEntryName, 
                                           PBORCA_TYPE otEntryType )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, 
			PBORCA_TYPE)) lpFunction[10])
		(m_hSession, lpszLibName, lpszEntryName, otEntryType);
}

PBWINAPI_(INT) CORCASession::LibraryEntryExport (LPTSTR       lpszLibName,  
                                           LPTSTR       lpszEntryName, 
                                           PBORCA_TYPE otEntryType,    
                                           LPTSTR       lpszExportBuffer, 
                                           LONG        lExportBufferSize )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, 
			PBORCA_TYPE, LPTSTR, LONG)) lpFunction[11])
		(m_hSession, lpszLibName, lpszEntryName, otEntryType,
			lpszExportBuffer, lExportBufferSize);
}

PBWINAPI_(INT) CORCASession::LibraryEntryInformation (LPTSTR lpszLibName,  
                                                LPTSTR lpszEntryName, 
                                                PBORCA_TYPE otEntryType,    
                                                PPBORCA_ENTRYINFO pEntryInfo )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, 
			PBORCA_TYPE, PPBORCA_ENTRYINFO)) lpFunction[12])
		(m_hSession, lpszLibName, lpszEntryName, otEntryType,
			pEntryInfo);
}

PBWINAPI_(INT) CORCASession::LibraryEntryMove (LPTSTR       lpszSourceLibName,  
                                         LPTSTR       lpszDestLibName,  
                                         LPTSTR       lpszEntryName, 
                                         PBORCA_TYPE otEntryType )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, LPTSTR,
			PBORCA_TYPE)) lpFunction[13])
		(m_hSession, lpszSourceLibName, lpszDestLibName, lpszEntryName,
			otEntryType);
}

PBWINAPI_(INT) CORCASession::CheckOutEntry (LPTSTR       lpszEntryName,
	  									LPTSTR       lpszSourceLibName,
	  									LPTSTR       lpszDestLibName,
	  									LPTSTR       lpszUserID,
                                    	PBORCA_TYPE otEntryType,
                                    	INT 		bMakeCopy)
{
	if(!m_hSession) return -1;

	return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, LPTSTR, LPTSTR,
			PBORCA_TYPE, INT)) lpFunction[14])
		(m_hSession, lpszEntryName, lpszSourceLibName, lpszDestLibName,
			lpszUserID, otEntryType, bMakeCopy);
}

PBWINAPI_(INT) CORCASession::CheckInEntry (LPTSTR       lpszEntryName,
	  									LPTSTR       lpszSourceLibName,
	  									LPTSTR       lpszDestLibName,
	  									LPTSTR       lpszUserID,
                                    	PBORCA_TYPE otEntryType,
                                    	INT 		bMoveEntry)
{
	if(!m_hSession) return -1;

	return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, LPTSTR, LPTSTR,
			PBORCA_TYPE, INT)) lpFunction[15])
		(m_hSession, lpszEntryName, lpszSourceLibName, lpszDestLibName,
			lpszUserID, otEntryType, bMoveEntry);
}

PBWINAPI_(INT) CORCASession::ListCheckOutEntries (LPTSTR lpszLibraryName,
	  										PBORCA_CHECKPROC	lpCallbackFunction,
	  										LPVOID      		pUserData)
{
	if(!m_hSession) return -1;

	return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, PBORCA_CHECKPROC, LPVOID)) lpFunction[16])
		(m_hSession, lpszLibraryName, lpCallbackFunction, pUserData);
}

PBWINAPI_(INT) CORCASession::CompileEntryImport (LPTSTR          lpszLibraryName,  
                                           LPTSTR          lpszEntryName, 
                                           PBORCA_TYPE    otEntryType, 
                                           LPTSTR          lpszComments,  
                                           LPTSTR          lpszEntrySyntax,
                                           LONG           lEntrySyntaxBuffSize, 
                                           PBORCA_ERRPROC pCompErrProc, 
                                           LPVOID         pUserData )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, PBORCA_TYPE,
		LPTSTR, LPTSTR, LONG, PBORCA_ERRPROC, LPVOID)) lpFunction[17])
		(m_hSession, lpszLibraryName, lpszEntryName, otEntryType,
			lpszComments, lpszEntrySyntax, lEntrySyntaxBuffSize, 
			pCompErrProc, pUserData);
}

PBWINAPI_(INT) CORCASession::CompileEntryImportList ( LPTSTR far* pLibraryNames,  
                                               LPTSTR far       *pEntryNames, 
                                               PBORCA_TYPE far *otEntryType, 
                                               LPTSTR far       *pComments,  
                                               LPTSTR far       *pEntrySyntaxBuffers,
                                               LONG far        *pEntrySyntaxBuffSizes, 
                                               INT              iNumberOfEntries, 
                                               PBORCA_ERRPROC   pCompErrProc, 
                                               LPVOID           pUserData )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR far*, LPTSTR far*, PBORCA_TYPE far*,
		LPTSTR far*, LPTSTR far*, LONG far*, INT, PBORCA_ERRPROC, LPVOID)) lpFunction[18])
		(m_hSession, pLibraryNames, pEntryNames, otEntryType,
			pComments, pEntrySyntaxBuffers, pEntrySyntaxBuffSizes, 
			iNumberOfEntries, pCompErrProc, pUserData);
}

PBWINAPI_(INT) CORCASession::CompileEntryRegenerate (LPTSTR lpszLibraryName,  
                                               LPTSTR          lpszEntryName, 
                                               PBORCA_TYPE    otEntryType, 
                                               PBORCA_ERRPROC pCompErrProc, 
                                               LPVOID         pUserData )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, PBORCA_TYPE,
		PBORCA_ERRPROC, LPVOID)) lpFunction[19])
		(m_hSession, lpszLibraryName, lpszEntryName, otEntryType,
			pCompErrProc, pUserData);
}

PBWINAPI_(INT) CORCASession::ApplicationRebuild (PBORCA_REBLD_TYPE	eRebldType,  
                                           	PBORCA_ERRPROC 		pCompErrProc, 
                                            LPVOID         		pUserData )
{
	if(!m_hSession) return -1;


	return ((INT (WINAPI *) (HPBORCA, PBORCA_REBLD_TYPE, PBORCA_ERRPROC, LPVOID)) lpFunction[20])
		(m_hSession, eRebldType, pCompErrProc, pUserData);
}

PBWINAPI_(INT) CORCASession::ExecutableCreate     (LPTSTR          lpszExeName,  
                                             LPTSTR          lpszIconName,        
                                             LPTSTR          lpszPBRName,        
	                                         PBORCA_LNKPROC pLinkErrProc,           
	                                         LPVOID         pUserData,              
                                             INT FAR       *iPBDFlags,  
                                             INT            iNumberOfPBDFlags,       
                                             LONG           lFlags )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, LPTSTR,
		PBORCA_LNKPROC, LPVOID, INT FAR*, INT, LONG)) lpFunction[21])
		(m_hSession, lpszExeName, lpszIconName, lpszPBRName,
			pLinkErrProc, pUserData, iPBDFlags, iNumberOfPBDFlags, lFlags);
}

PBWINAPI_(INT) CORCASession::DynamicLibraryCreate (LPTSTR lpszLibraryName,        
													LPTSTR lpszPBRName,            
													LONG lFlags )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, LONG)) lpFunction[22])
		(m_hSession, lpszLibraryName, lpszPBRName, lFlags);
}

PBWINAPI_(INT) CORCASession::ObjectQueryHierarchy (LPTSTR lpszLibraryName,  
                                             LPTSTR           lpszEntryName, 
                                             PBORCA_TYPE     otEntryType, 
                                             PBORCA_HIERPROC pHierarchyProc, 
                                             LPVOID          pUserData )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, PBORCA_TYPE,
		PBORCA_HIERPROC, LPVOID)) lpFunction[23])
		(m_hSession, lpszLibraryName, lpszEntryName, otEntryType,
			pHierarchyProc, pUserData);
}

PBWINAPI_(INT) CORCASession::ObjectQueryReference (LPTSTR           lpszLibraryName,  
                                             LPTSTR           lpszEntryName, 
                                             PBORCA_TYPE     otEntryType, 
                                             PBORCA_REFPROC  pReferenceProc, 
                                             LPVOID          pUserData )
{
	if(!m_hSession) return -1;

	return ((INT (WINAPI *) (HPBORCA, LPTSTR, LPTSTR, PBORCA_TYPE,
		PBORCA_REFPROC, LPVOID)) lpFunction[24])
		(m_hSession, lpszLibraryName, lpszEntryName, otEntryType,
			pReferenceProc, pUserData);
}


CORCASession::enumVersion CORCASession::Version()
{
	return m_version;
}

LPTSTR CORCASession::convertObjectType(UINT nType)
{
	switch(nType)
	{
	case PBORCA_APPLICATION:
		return "Application";
	case PBORCA_DATAWINDOW:
		return "DataWindow";
	case PBORCA_FUNCTION:
		return "Function";
	case PBORCA_MENU:
		return "Menu";
	case PBORCA_QUERY:
		return "Query";
	case PBORCA_STRUCTURE:
		return "Structure";
	case PBORCA_USEROBJECT:
		return "UserObject";
	case PBORCA_WINDOW:
		return "Window";
	case PBORCA_PIPELINE:
		return "Pipeline";
	case PBORCA_PROJECT:
		return "Project";
	case PBORCA_PROXYOBJECT:
		return "ProxyObject";
	default:
		return "<unknown>";
	}
}

pborca_type CORCASession::convertObjectType(LPTSTR lpszTypeName)
{
	if(strcmp(lpszTypeName, "Application") == 0) {
		return PBORCA_APPLICATION;
	} else if(strcmp(lpszTypeName, "DataWindow") == 0) {
		return PBORCA_DATAWINDOW;
	} else if(strcmp(lpszTypeName, "Function") == 0) {
		return PBORCA_FUNCTION;
	} else if(strcmp(lpszTypeName, "Menu") == 0) {
		return PBORCA_MENU;
	} else if(strcmp(lpszTypeName, "Query") == 0) {
		return PBORCA_QUERY;
	} else if(strcmp(lpszTypeName, "Structure") == 0) {
		return PBORCA_STRUCTURE;
	} else if(strcmp(lpszTypeName, "UserObject") == 0) {
		return PBORCA_USEROBJECT;
	} else if(strcmp(lpszTypeName, "Window") == 0) {
		return PBORCA_WINDOW;
	} else if(strcmp(lpszTypeName, "Pipeline") == 0) {
		return PBORCA_PIPELINE;
	} else if(strcmp(lpszTypeName, "Project") == 0) {
		return PBORCA_PROJECT;
	} else if(strcmp(lpszTypeName, "ProxyObject") == 0) {
		return PBORCA_PROXYOBJECT;
	} else return PBORCA_WINDOW;
}
