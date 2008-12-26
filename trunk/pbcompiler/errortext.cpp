
#include "stdafx.h"
#include "Pbcompiler.h"
#include "compiler.h"
#include "errortext.h"

CString GetErrorText(int ierrcode, _bstr_t libname, _bstr_t object)
{
	CString szMsg;			//вспомогательная переменная
	
	switch(ierrcode)
	{
	case PBORCA_INVALIDPARMS:
		szMsg.Format(IDSI_PBORCA10, (char *) object);
		return szMsg;
	case PBORCA_OBJNOTFOUND:
		szMsg.Format(IDSI_PBORCA11, (char *) object);
		return szMsg;
	case PBORCA_BADLIBRARY:
		szMsg.Format(IDSI_PBORCA12, (char *) libname);
		return szMsg;
	case PBORCA_LIBLISTNOTSET:
		szMsg.Format(IDSI_PBORCA13);
		return szMsg;
	case PBORCA_LIBNOTINLIST:
		szMsg.Format(IDSI_PBORCA14, (char *) libname);
		return szMsg;
	case PBORCA_LIBIOERROR:
		szMsg.Format(IDSI_PBORCA15, (char *) libname);
		return szMsg;
	case PBORCA_COMPERROR:
		szMsg.Format(IDSI_PBORCA15, (char *) object);
		return szMsg;
	};

	return "";
}

CString GetErrorText(_compile_error& Err)
{
	char szNo[32], szNo2[32];
	CString szMsg; // = "  Level: ";
	CString szLevel;

	//Уровень ошибки
	switch(Err.ilevel){
	case 0:
		szLevel = "0:ContextInfo";
		break;
	case 1:
		szLevel = "1:INFORMATION";
		break;
	case 2:
		szLevel = "2:OBSOLETE";
		break;
	case 3:
		szLevel = "3:WARNING";
		break;
	case 4:
		szLevel = "4:ERROR";
		break;
	case 5:
		szLevel = "5:FATAL";
		break;
	case 6:
		szLevel = "6:DBWARNING";
		break;
	};

	if(Err.icolnum > 0 || Err.ilinenum > 0)
	{
		szMsg.Format("%s %s %s (Col:%s, Line:%s)", szLevel, Err.szmsgnum, 
			Err.szmsgtxt, itoa(Err.icolnum, szNo, 10), itoa(Err.ilinenum, szNo2, 10));
	}
	else
	{
		szMsg.Format("%s %s\t%s", szLevel, Err.szmsgnum, Err.szmsgtxt);
	}

	return szMsg;
}
