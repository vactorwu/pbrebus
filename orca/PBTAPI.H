/*                                                                         */
/* *********************************************************************** */
/*                                                                         */
/*                 Copyright Powersoft Corporation 1991 - 1997             */
/*                                                                         */
/*       Powersoft Corporation ("Powersoft") claims copyright in this      */
/*     program and documentation as an unpublished work, versions of       */
/*     which were first licensed on the date indicated in the foregoing    */
/*     notice. Claim of copyright does not imply waiver of Powersoft's     */
/*     other rights. See Notice of Proprietary Rights.                     */
/*                                                                         */
/*                 NOTICE OF PROPRIETARY RIGHTS                            */
/*                                                                         */
/*       This program and documentation are confidential trade             */
/*     secrets and the property of Powersoft.  Use, examination,           */
/*     reproduction, copying, disassembly, decompilation, transfer         */
/*     and/or disclosure to others are strictly prohibited except by       */
/*     express written agreement with Powersoft.                           */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/*    Filename : pbtapi.h                                                  */
/*                                                                         */
/*    Purpose  : PowerBuilder TestTool API DLL public header file.         */
/*                                                                         */
/*    Notes    : 1. This header file should be included after <windows.h>  */
/*               2. Buffer lengths are in Characters NOT BYTES.            */
/*                  IE, in UNICODE each character is 2bytes.               */
/*                                                                         */
/*                                                                         */
/* *********************************************************************** */

#ifndef _INC_PB_TAPI_H                      /* First time defined?  */
#define _INC_PB_TAPI_H                      /* Yes, make sure of it */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DOUBLE                              /* We have a DOUBLE definiton? */
  #define DOUBLE      double                /* No, then do it */
  #define LPDOUBLE    double FAR *                 
#endif

#ifndef SHORT                               /* We have a SHORT definiton? */
  #define SHORT short                       /* No, then do it */
#endif


/* This PS_TIME struct _MUST_ match SH_TIME as defined in ...\include\shdt.h */
typedef struct                              /* Powersoft Time Structure */
{
	LONG    tm_msec;                        /* Micro seconds */
	SHORT   tm_year;                        /* Year - 1900 */
	BYTE    tm_mon;                         /* Month */
	BYTE    tm_mday;                        /* Day */
	BYTE    tm_hour;                        /* Hour */
	BYTE    tm_min;                         /* Minutes */
	BYTE    tm_sec;                         /* Seconds */
	BYTE    tm_filler;                      /* Fill out to even # of characters */
} PS_TIME, FAR *PPS_TIME;



/* *********************************************************************** */
/*  FNX_DataWindowDescribe                                                 */
/*                                                                         */
/*  Returns: Length of actual describe buffer (characters)                 */
/* *********************************************************************** */
ULONG FAR PASCAL 
FNX_DataWindowDescribe                   
( 
    HWND    hWnd,		                    /* Handle to DW */
	LPCTSTR	lpszDescribe,                   /* Ptr to input buffer */
	LPTSTR	lpszOutput,                     /* Ptr to output buffer */
	ULONG   lMaxLength                      /* Max length of output buffer (characters) */
);


/* *********************************************************************** */
/*  FNX_DataWindowGetBandAtPointer                                         */
/*                                                                         */
/*  Returns: Length of actual object name buffer (characters).             */
/* *********************************************************************** */
UINT FAR PASCAL 
FNX_DataWindowGetBandAtPointer           
( 
    HWND    hWnd,		                    /* Handle to DW */
	LPTSTR	lpszOutput,                     /* Ptr to output buffer */
	UINT    iMaxLength                      /* Max length of output buffer (characters)*/
);


/* *********************************************************************** */
/*  FNX_DataWindowGetChild                                                 */
/*                                                                         */
/*  Returns: HWND reference to child DW.                                   */
/* *********************************************************************** */
HWND FAR PASCAL 
FNX_DataWindowGetChild                   
( 
    HWND    hWnd,		                    /* Handle to DW */
	LPCTSTR	lpszChildName                   /* Ptr to child name */
);


/* *********************************************************************** */
/*  FNX_DataWindowGetCurrentColumn                                         */
/*                                                                         */
/*  Returns: Index of current column.                                      */
/* *********************************************************************** */
SHORT FAR PASCAL 
FNX_DataWindowGetCurrentColumn        
( 
    HWND    hWnd 		                    /* Handle to DW */
);


/* *********************************************************************** */
/*  FNX_DataWindowGetCurrentRow                                            */
/*                                                                         */
/*  Returns: Index of current row.                                         */
/* *********************************************************************** */
LONG FAR PASCAL 
FNX_DataWindowGetCurrentRow           
( 
    HWND    hWnd 		                    /* Handle to DW */
);


/* *********************************************************************** */
/*  FNX_DataWindowGetItemDateTime                                          */
/*                                                                         */
/*  Returns: -1 for failure, 1 for success                                 */
/* *********************************************************************** */
INT FAR PASCAL 
FNX_DataWindowGetItemDateTime                 
( 
    HWND       hWnd,                       /* Handle to DW */
    LONG       lRow,                       /* Row number */
    SHORT      iColumn,                    /* Column number */
    LPVOID     pDateTime                   /* Ptr to DateTime result */
                                           /* Pass a PPS_TIME for pDateTime */
);


/* *********************************************************************** */
/*  FNX_DataWindowGetItemNumber                                            */
/*                                                                         */
/*  Returns: -1 for failure, 1 for success                                 */
/* *********************************************************************** */
INT FAR PASCAL 
FNX_DataWindowGetItemNumber                   
( 
    HWND        hWnd,		                /* Handle to DW */
    LONG        lRow,                       /* Row number */
    SHORT       iColumn,                    /* Column number */
	LPDOUBLE    dValue                      /* Ptr to numeric result */
);


/* *********************************************************************** */
/*  FNX_DataWindowGetItemNumber                                            */
/*                                                                         */
/*  Returns: -1 for failure, 1 for success                                 */
/* *********************************************************************** */
INT FAR PASCAL 
FNX_DataWindowGetItemNumber                   
( 
    HWND        hWnd,		                /* Handle to DW */
    LONG        lRow,                       /* Row number */
    SHORT       iColumn,                    /* Column number */
	double FAR *dValue                      /* Numeric result */
);


/* *********************************************************************** */
/*  FNX_DataWindowGetItemString                                            */
/*                                                                         */
/*  Returns: -1 for failure, 1 for success                                 */
/* *********************************************************************** */
INT FAR PASCAL 
FNX_DataWindowGetItemString                   
( 
    HWND    hWnd,		                    /* Handle to DW */
    LONG    lRow,                           /* Row number */
    SHORT   iColumn,                        /* Column number */
	LPTSTR	lpszOutput,                     /* Ptr to output buffer */
	SHORT   iMaxLength                      /* Max length of output buffer (characters)*/
);


/* *********************************************************************** */
/*  FNX_DataWindowGetObjectAtPointer                                       */
/*                                                                         */
/*  Returns: Length of actual object name buffer(characters).                          */
/* *********************************************************************** */
UINT FAR PASCAL 
FNX_DataWindowGetObjectAtPointer           
( 
    HWND    hWnd,		                    /* Handle to DW */
	LPTSTR	lpszOutput,                     /* Ptr to output buffer */
	UINT    iMaxLength                      /* Max length of output buffer (characters)*/
);


/* *********************************************************************** */
/*  FNX_DataWindowGetRowCount                                              */
/*                                                                         */
/*  Returns: Total number of rows in DW.                                   */
/* *********************************************************************** */
LONG FAR PASCAL 
FNX_DataWindowGetRowCount             
( 
    HWND    hWnd 		                    /* Handle to DW */
);


/* *********************************************************************** */
/*  FNX_DataWindowSaveAs file type options                                 */
/* *********************************************************************** */
#define FNX_DW_SAVEAS_EXCEL         0 
#define FNX_DW_SAVEAS_TEXT          1
#define FNX_DW_SAVEAS_CSV           2
#define FNX_DW_SAVEAS_SYLK          3
#define FNX_DW_SAVEAS_WKS           4
#define FNX_DW_SAVEAS_WK1           5
#define FNX_DW_SAVEAS_DIF           6
#define FNX_DW_SAVEAS_DBF2          7
#define FNX_DW_SAVEAS_DBF3          8
#define FNX_DW_SAVEAS_SQL           9
#define FNX_DW_SAVEAS_CLIPBOARD    10
#define FNX_DW_SAVEAS_PS           11
#define FNX_DW_SAVEAS_WMF          12

/* *********************************************************************** */
/*  FNX_DataWindowSaveAs                                                   */
/*                                                                         */
/*  Returns: -1 for failure, 1 for success                                 */
/* *********************************************************************** */
INT FAR PASCAL 
FNX_DataWindowSaveAs	                    
( 
    HWND     hWnd,                           /* Handle to DW */
    INT      iSaveAsType,                    /* SaveAsType */
    LPCTSTR  lpszFileName,                   /* Ptr to file name */
    BOOL     bIncludeHeadings                /* Include headings flag */
);


/* *********************************************************************** */
/*  FNX_DataWindowSelectRow                                                */
/*                                                                         */
/*  Returns: -1 for failure, 1 for success                                 */
/* *********************************************************************** */
INT FAR PASCAL 
FNX_DataWindowSelectRow           
( 
    HWND    hWnd,		                    /* Handle to DW */
    LONG    lRow,                           /* Row number */
    BOOL    bSelectState                    /* Select state */
);


/* *********************************************************************** */
/*  FNX_DataWindowSetAllEvents                                                */
/*                                                                         */
/*  Returns: -1 for failure, 1 for success                                 */
/* *********************************************************************** */
INT FAR PASCAL 
FNX_DataWindowSetAllEvents
(
    HWND    hWnd                            /* Handle to DW */
);

/* *********************************************************************** */
/*  FNX_DataWindowSetCurrentColumn                                         */
/*                                                                         */
/*  Returns: -1 for failure, 1 for success                                 */
/* *********************************************************************** */
INT FAR PASCAL 
FNX_DataWindowSetCurrentColumn        
( 
    HWND    hWnd,		                    /* Handle to DW */
    SHORT   iColumn                         /* Column number */
);


/* *********************************************************************** */
/*  FNX_DataWindowSetCurrentRow                                            */
/*                                                                         */
/*  Returns: -1 for failure, 1 for success                                 */
/* *********************************************************************** */
INT FAR PASCAL 
FNX_DataWindowSetCurrentRow           
( 
    HWND    hWnd,		                    /* Handle to DW */
    LONG    lRow                            /* Row number */
);


/* *********************************************************************** */
/*  FNX_IsPowerObject                                                      */
/*                                                                         */
/*  Returns: FALSE for failure, TRUE for success                           */
/* *********************************************************************** */
BOOL FAR PASCAL 
FNX_IsPowerObject		                    /* Test for PowerBuilder objects */
( 
    HWND    hWnd                            /* Handle to test window */ 
);


/* *********************************************************************** */
/*  FNX_ObjectClassName                                                    */
/*                                                                         */
/*  Returns: 0 for failure, > 0 for success ( actual length of name )      */
/* *********************************************************************** */
INT FAR PASCAL 
FNX_ObjectClassName		                    /* Retrieve object class name */
( 
    HWND    hWnd,                           /* Handle to window */
    LPTSTR  lpszNameBuffer,                 /* Ptr to name buffer */
    INT     iNameBufferLength               /* Max name buffer length (characters)*/
);


/* *********************************************************************** */
/*  FNX_ObjectIUnknown                                                     */
/*                                                                         */
/*  Returns:  1 for success; -1 for failure                                */
/* *********************************************************************** */
INT FAR PASCAL 
FNX_ObjectIUnknown
(
   HWND     hWnd,                           /* Handle to window */
   VOID FAR * FAR * ppvObject               /* Storage location for IUnknown */
);

/* *********************************************************************** */
/* DataWindow Control Notification Codes                                   */
/* Any RC=0 is the default action if no script is coded.                   */
/* *********************************************************************** */

/* ITEMCHANGE issued after an item has passed its validation test */
/* but before it has been put into the dw table. */
/* RC=0; means accept the data value */
/* RC=1; means reject the data value */
#define FNX_DWN_ITEMCHANGE             0x0100
/* ITEMVALIDATEIONERROR issued when an item fails its validation test. */
/* RC=0; means reject data value and show message box */
/* RC=1; means reject data value with no message box */
/* RC=2; means accept data value */
#define FNX_DWN_ITEMVALIDATIONERROR    0x0200
/* RETRIEVESTART issued before starting any database retrieval.  It */
/* can be used to set the database transaction parameters. */
/* RC=0; continue */
/* RC=1; don't do retrieval */
#define FNX_DWN_RETRIEVESTART          0x0300
/* RETRIEVEEND issued after all rows have been retrieved. */
/* RC=0; continue */
#define FNX_DWN_RETRIEVEEND            0x0301
/* RETRIEVEROW issued after a row has been retrieved and placed in the */
/* dw table. */
/* RC=0; continue */
/* RC=1; stop retrieval */
#define FNX_DWN_RETRIEVEROW            0x0302
/* UPDATESTART issued before starting any database update.  It can be used */
/* to set the database transaction parameters. */
/* RC=0; continue */
/* RC=1; don't do update */
#define FNX_DWN_UPDATESTART            0x0400
/* UPDATEEND issued after all database update.  A COMMIT has been done */
/* prior to this message except for user defined transactions.  A user */
/* must do their own commit if they supplied the transaction. */
/* RC=0; continue */
#define FNX_DWN_UPDATEEND			   0x0401
/* LBUTTONDBLCLK issued when user double clicks.  Note: there is no */
/* guarentee that currency exists or is visible on the window.  If the user */
/* has clicked on top of a field then this field is made current before the */
/* LBUTTONDBLCLK is issued.  If the click does not occur on top of a field */
/* then whatever currency that existed prior to the request (if any) is */
/* maintained.*/
#define FNX_DWN_LBUTTONDBLCLK          0x0500
#define FNX_DWN_LBUTTONCLK             0x0501
#define FNX_DWN_RBUTTONDBLCLK          0x0502
#define FNX_DWN_RBUTTONCLK             0x0503
#define FNX_DWN_MBUTTONDBLCLK          0x0504
#define FNX_DWN_MBUTTONCLK             0x0505
/* DBERROR issued when a database error is encountered.  The message is */
/* issued after a ROLLBACK is issued (unless this is a user specified transaction) */
/* and before a message box is displayed.  If this is a user specified */
/* transaction the user is responsible for issuing the ROLLBACK. */
/* RC=0; display error message */
/* RC=1; don't display error message */
#define FNX_DWN_DBERROR                0x0600
/* ITEMCHANGEFOCUS issued when a new item gets focus */
#define FNX_DWN_ITEMCHANGEFOCUS        0x0700
/* ROWCHANGE issued when the current row changes.  Use GetRow to find out */
/* what that row is.  This is primarily useful for listbox style processing. */
#define FNX_DWN_ROWCHANGE              0x0800
/* SELCHANGE issued when a row selection changes. */
#define FNX_DWN_SELCHANGE			   0x0801	
/* TABOUT issued when user tabs out of DataWindow. */
#define FNX_DWN_TABOUT                 0x0900
#define FNX_DWN_BACKTABOUT             0x0901
#define FNX_DWN_TABUPOUT               0x0902
#define FNX_DWN_TABDOWNOUT             0x0903
/* Return 1 when you want DW to ignore the enter key */
#define FNX_DWN_PROCESSENTER		   0x0904
/* New events for release 2 */
#define FNX_DWN_KILLFOCUS			   0x0905
#define FNX_DWN_SETFOCUS			   0x0906
#define FNX_DWN_MOUSEMOVE			   0x0907
#define FNX_DWN_LBUTTONDOWN			   0x0908
#define FNX_DWN_LBUTTONUP			   0x0909
#define FNX_DWN_RBUTTONDOWN			   0x090A
#define FNX_DWN_RBUTTONUP			   0x090B
#define FNX_DWN_CHANGING			   0x090C
#define FNX_DWN_HSCROLL				   0x090D
#define FNX_DWN_VSCROLL				   0x090E
/* SQL issued prior to any syntax passed to the database engine. */
/* User can inspect the syntax using "dwSQLGetSyntax" and modify */
/* it using "dwSQLSetSyntax".  Event is triggered by issuing */
/* RETRIEVE, UPDATE, RESELECT. */
/* RC=0; continue */
/* RC=1; fail */
/* RC=2; skip and continue */
#define FNX_DWN_SQL					   0x090F
#define FNX_DWN_RESIZE				   0x0910
#define FNX_DWN_MOVE				   0x0911
#define FNX_DWN_SYSCHAR				   0x0912
/* Print events */
#define FNX_DWN_PRINTSTART			   0x0913
/* Print page occurs before printing the page. */
/* RC=0; continue */
/* RC=1; skip page */
#define FNX_DWN_PRINTPAGE			   0x0914
#define FNX_DWN_PRINTEND			   0x0915
/* FNX_DWN_KEY is really on WM_KEYDOWN */
#define FNX_DWN_KEY					   0x0916
/* DDDW and DDLB when drop down is about to be shown this event is triggered */
/* Return 1 when you want to disallow the drop down. */
#define FNX_DWN_DROPDOWN			   0x0917
/* Graph has just been built when this event is triggered */
#define FNX_DWN_GRAPHCREATE			   0x0918
/* Margin has been dragged by the user */
#define FNX_DWN_PRINTMARGINCHANGE	   0x0919
/* Non-client area messages */
#define FNX_DWN_NCHITTEST              0x0920
#define FNX_DWN_NCRBUTTONDOWN          0x0921
/* Notify message that tells you that message text is availale - get text by using */
/* dwMessageText function */
#define FNX_DWN_MESSAGETEXT            0x922
/* Nest needs a transaction for retrieval */
#define FNX_DWN_NEED_TRANS             0x923

/* More non-client area messages */
#define FNX_DWN_NCLBUTTONDOWN          0x0923
#define FNX_DWN_NCLBUTTONDBLCLK        0x0924

/* Some events used by PB runtime */
#define FNX_DWN_DRAGDROP			   0x0A00
#define FNX_DWN_DRAGENTER			   0x0A01
#define FNX_DWN_DRAGLEAVE			   0x0A02
#define FNX_DWN_DRAGWITHIN			   0x0A03


//*********************************************************************** 
//* FNX_Rte - RichTextEdit Functions that work against either a RichTextEdit
//*           control or a Datawindow defined with RichTextEdit presentation
//*           style                                                    
//*                                                                         
//*********************************************************************** 

#define FNX_RTEBAND_HEADER  1
#define FNX_RTEBAND_FOOTER  2
#define FNX_RTEBAND_DETAIL  3

#define FNX_CFE_BOLD        0x0001
#define FNX_CFE_UNDERLINE   0x0002
#define FNX_CFE_ITALIC      0x0004
#define FNX_CFE_STRIKEOUT   0x0008
#define FNX_CFE_SUBSCRIPT   0x0010
#define FNX_CFE_SUPERSCRIPT 0x0020

typedef struct  fnx_charformat                     
{
  UINT         cbSize;                 // size of this structure (bytes)
  DWORD        dwMask;                 // Used for SetCharFormat .. currently not supported
  DWORD        dwEffects;              // GetCharFormat characteristics defined using FNX_CFE_ values
  LONG         yHeight;
  COLORREF     crTextColor;
  BYTE         bCharSet;
  BYTE         bPitchAndFamily;
  TCHAR        szFaceName[LF_FACESIZE];
} FNXCHARFORMAT, FAR *PFNXCHARFORMAT;

INT FAR PASCAL FNX_RteGetCharFormat         // Get character format based on current
(                                           // cursor position in the control 
    HWND    hWnd,                           // Handle to window 
    LPVOID  pRTECharFormat                  // Ptr to CharFormat Structure
);

INT FAR PASCAL FNX_RteGetCurrentBand
(
    HWND    hWnd
);

INT FAR PASCAL FNX_RteGetLine               // Return text of given line 
( 
    HWND    hWnd,                           // Handle to window 
    LONG    lLine,                          // Line number
    LPTSTR  lpszLineBuffer,                 // Ptr to line buffer 
    INT     iLineBufferLength               // Max line buffer length (characters)
);

INT FAR PASCAL FNX_RteGetSelection          // Get selection "coordinates" 
( 
    HWND    hWnd,                           // Handle to window 
    LPLONG  lpFromLine,
    LPLONG  lpFromCol,
    LPLONG  lpToLine,
    LPLONG  lpToCol
);

INT FAR PASCAL FNX_RteLineCount             // Return number of lines in control 
( 
    HWND    hWnd                            // Handle to window 
);

INT FAR PASCAL FNX_RteLineLength            // Return length of given line 
( 
    HWND    hWnd,                           // Handle to window 
    LONG    lLine                           // Line number 
);

INT FAR PASCAL FNX_RteSetCurrentBand        // Set current band type:
                                            //   FNX_RTEBAND_DETAIL
                                            //   FNX_RTEBAND_HEADER
                                            //   FNX_RTEBAND_FOOTER
                                            // Note: care should be taken to reset the band
                                            // type after processing has finished (odd things
                                            // could potentially happen to the user if current
                                            // band type is no longer on the window they're 
                                            // looking at!)
(
    HWND    hWnd,
    INT     nBand
);        


INT FAR PASCAL FNX_RteSetSelection          // Set selection "coordinates"
( 
    HWND    hWnd,                           // Handle to window 
    LONG    lFromLine,                         
    LONG    lFromCol,                         
    LONG    lToLine,                         
    LONG    lToCol                         
);

#ifdef __cplusplus
}; // extern "C"
#endif

#endif    /* _INC_PB_TAPI_H */


