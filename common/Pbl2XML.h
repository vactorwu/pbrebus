// Pbl2XML.h: interface for the CPbl2XML class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PBL2XML_H__37333643_B7BA_4CB9_BA10_5C80E21E3B28__INCLUDED_)
#define AFX_PBL2XML_H__37333643_B7BA_4CB9_BA10_5C80E21E3B28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPbl2XML  
{
public:
	bool Convert(MSXML2::IXMLDOMElementPtr &library, _bstr_t &);

	CPbl2XML(CORCASession &orca);
	virtual ~CPbl2XML();

protected:
	CORCASession &m_orca;
};

#endif // !defined(AFX_PBL2XML_H__37333643_B7BA_4CB9_BA10_5C80E21E3B28__INCLUDED_)
