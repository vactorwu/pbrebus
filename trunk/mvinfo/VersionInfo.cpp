// VersionInfo.cpp : Implementation of CVersionInfo
#include "stdafx.h"
#include "Mvinfo.h"
#include "VersionInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CVersionInfo
using namespace vi;

STDMETHODIMP CVersionInfo::load(BSTR lpszFileName)
{
	if(pvi_) delete pvi_;

	if(mode == 1)
		pvi_ = new VersionInfoBinary<VIParserSimple, CVIErrorHandler>;
	else
		pvi_ = new vi::VersionInfo<VIParserSimple, CVIErrorHandler>;

	if(!pvi_->load((char *) _bstr_t(lpszFileName))) return S_FALSE;

	return S_OK;
}

STDMETHODIMP CVersionInfo::get_FileVersion(BSTR *pVal)
{
	if(!pvi_) return S_FALSE;

	*pVal = _bstr_t((*pvi_)[vi::VIH_FILEVERSION]).copy();
	
	return S_OK;
}

STDMETHODIMP CVersionInfo::put_FileVersion(BSTR newVal)
{
	if(!pvi_) return S_FALSE;

	(*pvi_)[vi::VIH_FILEVERSION] = newVal;
	(*pvi_)[VI_FILEVERSION] = newVal;

	pvi_->update();

	return S_OK;
}

STDMETHODIMP CVersionInfo::get_ProductVersion(BSTR *pVal)
{
	if(!pvi_) return S_FALSE;

	*pVal = _bstr_t((*pvi_)[vi::VIH_PRODUCTVERSION]).copy();

	return S_OK;
}

STDMETHODIMP CVersionInfo::put_ProductVersion(BSTR newVal)
{
	if(!pvi_) return S_FALSE;

	(*pvi_)[vi::VIH_PRODUCTVERSION] = newVal;
	(*pvi_)[VI_PRODUCTVERSION] = newVal;

	pvi_->update();

	return S_OK;
}

STDMETHODIMP CVersionInfo::get_Mode(short *pVal)
{
	*pVal = mode;

	return S_OK;
}

STDMETHODIMP CVersionInfo::put_Mode(short newVal)
{
	mode = newVal;

	return S_OK;
}

STDMETHODIMP CVersionInfo::get_Description(BSTR *pVal)
{
	if(!pvi_) return S_FALSE;

	*pVal = _bstr_t((*pvi_)[VI_FILEDESCRIPTION]).copy();

	return S_OK;
}

STDMETHODIMP CVersionInfo::put_Description(BSTR newVal)
{
	if(!pvi_) return S_FALSE;

	(*pvi_)[VI_FILEDESCRIPTION] = newVal;

	pvi_->update();

	return S_OK;
}

STDMETHODIMP CVersionInfo::get_Copyright(BSTR *pVal)
{
	if(!pvi_) return S_FALSE;

	*pVal = _bstr_t((*pvi_)[VI_LEGALCOPYRIGHT]).copy();

	return S_OK;
}

STDMETHODIMP CVersionInfo::put_Copyright(BSTR newVal)
{
	if(!pvi_) return S_FALSE;

	(*pvi_)[VI_LEGALCOPYRIGHT] = newVal;

	pvi_->update();

	return S_OK;
}

STDMETHODIMP CVersionInfo::get_CompanyName(BSTR *pVal)
{
	if(!pvi_) return S_FALSE;

	*pVal = _bstr_t((*pvi_)[VI_COMPANYNAME]).copy();

	return S_OK;
}

STDMETHODIMP CVersionInfo::put_CompanyName(BSTR newVal)
{
	if(!pvi_) return S_FALSE;

	(*pvi_)[VI_COMPANYNAME] = newVal;

	pvi_->update();

	return S_OK;
}

STDMETHODIMP CVersionInfo::get_InternalName(BSTR *pVal)
{
	if(!pvi_) return S_FALSE;

	*pVal = _bstr_t((*pvi_)[VI_INTERNALNAME]).copy();

	return S_OK;
}

STDMETHODIMP CVersionInfo::put_InternalName(BSTR newVal)
{
	if(!pvi_) return S_FALSE;

	(*pvi_)[VI_INTERNALNAME] = newVal;

	pvi_->update();

	return S_OK;
}

STDMETHODIMP CVersionInfo::get_ProductName(BSTR *pVal)
{
	if(!pvi_) return S_FALSE;

	*pVal = _bstr_t((*pvi_)[VI_PRODUCTNAME]).copy();

	return S_OK;
}

STDMETHODIMP CVersionInfo::put_ProductName(BSTR newVal)
{
	if(!pvi_) return S_FALSE;

	(*pvi_)[VI_PRODUCTNAME] = newVal;

	pvi_->update();

	return S_OK;
}

STDMETHODIMP CVersionInfo::get_Comments(BSTR *pVal)
{
	if(!pvi_) return S_FALSE;

	*pVal = _bstr_t((*pvi_)[VI_COMMENTS]).copy();

	return S_OK;
}

STDMETHODIMP CVersionInfo::put_Comments(BSTR newVal)
{
	if(!pvi_) return S_FALSE;

	(*pvi_)[VI_COMMENTS] = newVal;

	pvi_->update();

	return S_OK;
}

STDMETHODIMP CVersionInfo::get_LegalTrademarks(BSTR *pVal)
{
	if(!pvi_) return S_FALSE;

	*pVal = _bstr_t((*pvi_)[VI_LEGALTRADEMARKS]).copy();

	return S_OK;
}

STDMETHODIMP CVersionInfo::put_LegalTrademarks(BSTR newVal)
{
	if(!pvi_) return S_FALSE;

	(*pvi_)[VI_LEGALTRADEMARKS] = newVal;

	pvi_->update();

	return S_OK;
}

STDMETHODIMP CVersionInfo::get_OriginalFilename(BSTR *pVal)
{
	if(!pvi_) return S_FALSE;

	*pVal = _bstr_t((*pvi_)[VI_ORIGINALFILENAME]).copy();

	return S_OK;
}

STDMETHODIMP CVersionInfo::put_OriginalFilename(BSTR newVal)
{
	if(!pvi_) return S_FALSE;

	(*pvi_)[VI_ORIGINALFILENAME] = newVal;

	pvi_->update();

	return S_OK;
}

STDMETHODIMP CVersionInfo::get_SpecialBuild(BSTR *pVal)
{
	if(!pvi_) return S_FALSE;

	*pVal = _bstr_t((*pvi_)[VI_SPECIALBUILD]).copy();

	return S_OK;
}

STDMETHODIMP CVersionInfo::put_SpecialBuild(BSTR newVal)
{
	if(!pvi_) return S_FALSE;

	(*pvi_)[VI_SPECIALBUILD] = newVal;

	pvi_->update();

	return S_OK;
}

STDMETHODIMP CVersionInfo::setProperty(BSTR name, BSTR val)
{
	if(!pvi_) return S_FALSE;

	VIItem &item = (*pvi_)[name];

	if(item == VIItem::dummy) return S_FALSE;

	item = val;

	pvi_->update();

	return S_OK;
}

STDMETHODIMP CVersionInfo::getProperty(BSTR name, BSTR *pVal)
{
	if(!pvi_) return S_FALSE;

	*pVal = _bstr_t((*pvi_)[name]).copy();

	return S_OK;
}
