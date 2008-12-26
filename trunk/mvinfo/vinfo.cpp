//////////////////////////////////////////////////////////////////////////////////////////
// Version info - wrapper for version resource of a module. It allows to change 
//				  version info fields (file version, product version, comment, product 
//				  name and so on).
//
//				  Functions like SetFileVersionInfo(...) or VarSetValue(...) are absent
//				  in WinAPI, but some time you need to update version info 
//				  programatically. That is why this classes were developed.
//
//				  Evgeny Savitsky @ 2003, savitsky@init-s.ru
//
#include "stdafx.h"
#include "vinfo.h"

using namespace vi;

VIHeaderItem VIHeaderItem::dummy;	// empty object for opertaor[]
VIItem VIItem::dummy; // dummy object for operator[]


BOOL CALLBACK vi::proc(HMODULE hModule, LPCTSTR lpType,
        LPCTSTR lpName, WORD  wLanguage, LONG lParam)
{
	return TRUE;
}

//
// Evgeny Savitsky @ 2003, savitsky@init-s.ru
//////////////////////////////////////////////////////////////////////////////////////////