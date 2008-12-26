// ORCASession.h: interface for the CORCASession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORCASESSION_H__04FF572E_CF6D_48EA_A896_31D771FDF3B2__INCLUDED_)
#define AFX_ORCASESSION_H__04FF572E_CF6D_48EA_A896_31D771FDF3B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PBORCA.h"

#define FUNCTIONS_NUM 25

class CORCASession
{
	LPVOID lpFunction[FUNCTIONS_NUM];

public:
	HPBORCA m_hSession;
	enum enumVersion {PB_6, PB_8, PB_9, NONE};

public:
	bool Use(enumVersion ver);
	bool Use(LPCSTR);

	bool SessionUse(HPBORCA);

public:
	enumVersion Version();
	CORCASession();
	virtual ~CORCASession();

	PBWINAPI_(VOID) SessionClose ();
	PBWINAPI_(VOID) SessionGetError (LPTSTR   lpszErrorBuffer,
                                      INT     iErrorBufferSize );
	PBWINAPI_(VOID) SessionOpen( void );
	PBWINAPI_(INT) SessionSetCurrentAppl (LPTSTR   lpszApplLibName,  
                                           LPTSTR   lpszApplName );

	PBWINAPI_(INT) SessionSetLibraryList (LPTSTR FAR *pLibNames, 
											INT iNumberOfLibs );

	PBWINAPI_(INT) LibraryCommentModify (LPTSTR   lpszLibName,  
                                             LPTSTR   lpszLibComments );

	PBWINAPI_(INT) LibraryCreate    (LPTSTR   lpszLibName,  
                                         LPTSTR   lpszLibComments );

	PBWINAPI_(INT) LibraryDelete    (LPTSTR   lpszLibName ); 
	
	PBWINAPI_(INT) LibraryDirectory (LPTSTR           lpszLibName,       
                                         LPTSTR           lpszLibComments,  
                                         INT             iCmntsBuffSize, 
                                         PBORCA_LISTPROC pListProc, 
                                         LPVOID          pUserData );

	PBWINAPI_(INT) LibraryEntryCopy (LPTSTR       lpszSourceLibName,  
                                         LPTSTR       lpszDestLibName,  
                                         LPTSTR       lpszEntryName, 
                                         PBORCA_TYPE otEntryType );

	PBWINAPI_(INT) LibraryEntryDelete (LPTSTR       lpszLibName,  
                                           LPTSTR       lpszEntryName, 
                                           PBORCA_TYPE otEntryType );

	PBWINAPI_(INT) LibraryEntryExport (LPTSTR       lpszLibName,  
                                           LPTSTR       lpszEntryName, 
                                           PBORCA_TYPE otEntryType,    
                                           LPTSTR       lpszExportBuffer, 
                                           LONG        lExportBufferSize );

	PBWINAPI_(INT) LibraryEntryInformation (LPTSTR             lpszLibName,  
                                                LPTSTR             lpszEntryName, 
                                                PBORCA_TYPE       otEntryType,    
                                                PPBORCA_ENTRYINFO pEntryInfo );

	PBWINAPI_(INT) LibraryEntryMove (LPTSTR       lpszSourceLibName,  
                                         LPTSTR       lpszDestLibName,  
                                         LPTSTR       lpszEntryName, 
                                         PBORCA_TYPE otEntryType );

	PBWINAPI_(INT) CheckOutEntry (LPTSTR       lpszEntryName,
	  									LPTSTR       lpszSourceLibName,
	  									LPTSTR       lpszDestLibName,
	  									LPTSTR       lpszUserID,
                                    	PBORCA_TYPE otEntryType,
                                    	INT 		bMakeCopy);

	PBWINAPI_(INT) CheckInEntry (LPTSTR       lpszEntryName,
	  									LPTSTR       lpszSourceLibName,
	  									LPTSTR       lpszDestLibName,
	  									LPTSTR       lpszUserID,
                                    	PBORCA_TYPE otEntryType,
                                    	INT 		bMoveEntry);

	PBWINAPI_(INT) ListCheckOutEntries (LPTSTR       		lpszLibraryName,
	  										PBORCA_CHECKPROC	lpCallbackFunction,
	  										LPVOID      		pUserData);

	PBWINAPI_(INT) CompileEntryImport (    LPTSTR          lpszLibraryName,  
                                           LPTSTR          lpszEntryName, 
                                           PBORCA_TYPE    otEntryType, 
                                           LPTSTR          lpszComments,  
                                           LPTSTR          lpszEntrySyntax,
                                           LONG           lEntrySyntaxBuffSize, 
                                           PBORCA_ERRPROC pCompErrProc, 
                                           LPVOID         pUserData );

	PBWINAPI_(INT) CompileEntryImportList (    LPTSTR far       *pLibraryNames,  
                                               LPTSTR far       *pEntryNames, 
                                               PBORCA_TYPE far *otEntryType, 
                                               LPTSTR far       *pComments,  
                                               LPTSTR far       *pEntrySyntaxBuffers,
                                               LONG far        *pEntrySyntaxBuffSizes, 
                                               INT              iNumberOfEntries, 
                                               PBORCA_ERRPROC   pCompErrProc, 
                                               LPVOID           pUserData );

	PBWINAPI_(INT) CompileEntryRegenerate (    LPTSTR          lpszLibraryName,  
                                               LPTSTR          lpszEntryName, 
                                               PBORCA_TYPE    otEntryType, 
                                               PBORCA_ERRPROC pCompErrProc, 
                                               LPVOID         pUserData );

	PBWINAPI_(INT) ApplicationRebuild (    	PBORCA_REBLD_TYPE	eRebldType,  
                                           	PBORCA_ERRPROC 		pCompErrProc, 
                                            LPVOID         		pUserData );

	PBWINAPI_(INT) ExecutableCreate     (LPTSTR          lpszExeName,  
                                             LPTSTR          lpszIconName,        
                                             LPTSTR          lpszPBRName,        
	                                         PBORCA_LNKPROC pLinkErrProc,           
	                                         LPVOID         pUserData,              
                                             INT FAR       *iPBDFlags,  
                                             INT            iNumberOfPBDFlags,       
                                             LONG           lFlags );

	PBWINAPI_(INT) DynamicLibraryCreate (LPTSTR          lpszLibraryName,        
                                             LPTSTR          lpszPBRName,            
                                             LONG           lFlags );

	PBWINAPI_(INT) ObjectQueryHierarchy (LPTSTR           lpszLibraryName,  
                                             LPTSTR           lpszEntryName, 
                                             PBORCA_TYPE     otEntryType, 
                                             PBORCA_HIERPROC pHierarchyProc, 
                                             LPVOID          pUserData );

	PBWINAPI_(INT) ObjectQueryReference (LPTSTR           lpszLibraryName,  
                                             LPTSTR           lpszEntryName, 
                                             PBORCA_TYPE     otEntryType, 
                                             PBORCA_REFPROC  pReferenceProc, 
                                             LPVOID          pUserData );

	LPTSTR convertObjectType(UINT);
	pborca_type convertObjectType(LPTSTR);

protected:
	enumVersion m_version;
	HMODULE m_hLibrary;
};

#endif // !defined(AFX_ORCASESSION_H__04FF572E_CF6D_48EA_A896_31D771FDF3B2__INCLUDED_)
