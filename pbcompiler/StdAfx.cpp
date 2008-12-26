// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

#include <atlimpl.cpp>

// logger
CLogFile log;

// PB ORCA library
CORCASession orca;

// windows registry access
CRegistry registry("PBDevEx");

LPCTSTR gpszProductSalt2 = "{B765C842-66B5-4374-A1AE-2CF623939E5B}";
