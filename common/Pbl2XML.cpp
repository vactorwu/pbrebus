// Pbl2XML.cpp: implementation of the CPbl2XML class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\orca\ORCASession.h"
#include "msxml3.tlh"
#include "cstring.h"
#include "Pbl2XML.h"
#include <time.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPbl2XML::CPbl2XML(CORCASession &orca) : m_orca(orca)
{
}

CPbl2XML::~CPbl2XML()
{

}

void CALLBACK LibDir(PPBORCA_DIRENTRY pEntry, void * lpData)
{
	MSXML2::IXMLDOMElementPtr childs((MSXML2::IXMLDOMElement *) lpData);
	
	MSXML2::IXMLDOMElementPtr object = childs->ownerDocument->createElement("object");
	
	object->setAttribute(_bstr_t("name"), _bstr_t(pEntry->lpszEntryName));
	object->setAttribute(_bstr_t("comment"), _bstr_t(pEntry->szComments));

	char lsnumber[30] = {0};
	itoa(pEntry->otEntryType, lsnumber, 10);
	object->setAttribute(_bstr_t("type"), _bstr_t(lsnumber));
	
	object->setAttribute(_bstr_t("typename"), 
		_bstr_t(orca.convertObjectType(pEntry->otEntryType)));

	itoa(pEntry->lEntrySize, lsnumber, 10);
	object->setAttribute(_bstr_t("size"), _bstr_t(lsnumber));

	tm *ptime = localtime(&pEntry->lCreateTime);
	strftime(lsnumber, sizeof(lsnumber), "%d.%m.%y %H:%M:%S", ptime);
	object->setAttribute(_bstr_t("createtime"), _bstr_t(lsnumber));

	childs->appendChild(object);
	object = NULL;
}

bool CPbl2XML::Convert(MSXML2::IXMLDOMElementPtr &library, _bstr_t &path)
{
	MSXML2::IXMLDOMElementPtr childnode = library->ownerDocument->
		createElement("childs");

	char buffer[256] = {0};

	int iResult = m_orca.LibraryDirectory((char *) path, 
		buffer, sizeof(buffer),	LibDir, (LPVOID) childnode.GetInterfacePtr());

	if(iResult != PBORCA_OK) {
		CString msg; msg.Format("FAILED: LibraryDirectory(...) : %s", (char *) path);
		log.write(msg);
		
		childnode = NULL;
		return false;
	}

	_bstr_t xpath("childs");
	MSXML2::IXMLDOMNodeListPtr childlist = library->selectNodes(xpath);

	if(childlist->length > 0) {
		library->replaceChild(childnode, childlist->item[0]);
	} else {
		library->appendChild(childnode);
	}

	// сохран€ем значение комментари€ библиотеки
	library->setAttribute(_bstr_t("comment"), _bstr_t(buffer));

	return true;
}
