// SourceCodeControl.cpp : Implementation of CSourceCodeControl
#include "stdafx.h"
#include "Rebusscc.h"
#include "SourceCodeControl.h"

/////////////////////////////////////////////////////////////////////////////
// CSourceCodeControl

STDMETHODIMP CSourceCodeControl::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ISourceCodeControl
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CSourceCodeControl::RaiseError( double number, _bstr_t description, 
										_bstr_t source )
{
	ICreateErrorInfo *pCrErrInfo;
	HRESULT hr;
	HRESULT res;
	bool succ = true;
	//Преобразовываем тип кода ошибки
	hr =VarUI4FromR8(number, (ULONG*)&res);
	if (FAILED(hr))
	{
		hr = VarI4FromR8(number, &res);
		if (FAILED(hr))
		{
			res = 0x80040401;
			succ = false;
		}
	}

	//Получаем интерфейс для создания информации о ошибке
	hr = CreateErrorInfo(&pCrErrInfo);
	if (FAILED(hr))
	{
		return hr;
	}
	//Устанавливаем параметры ошибки
	if (succ)
	{
		pCrErrInfo->SetDescription(description);
		pCrErrInfo->SetGUID(IID_IDispatch);
		pCrErrInfo->SetHelpContext(0);
		pCrErrInfo->SetHelpFile(L"");
		pCrErrInfo->SetSource(source);
	}
	else
	{
		char mess[100];
		sprintf(mess,"Failed to convert %.0f.",number);
		_bstr_t text(mess);
		pCrErrInfo->SetDescription(text);
		pCrErrInfo->SetGUID(IID_IDispatch);
		pCrErrInfo->SetHelpContext(0);
		pCrErrInfo->SetHelpFile(L"");
		pCrErrInfo->SetSource(L"");
	}

	IErrorInfo *pErrInfo;
	//Получаем интерфейс для установки значений ошибок
	hr = pCrErrInfo->QueryInterface(IID_IErrorInfo, (LPVOID FAR*)&pErrInfo);

	if (FAILED(hr))
	{
		return hr;
	}
	//Устанавливает информацию об ошибке
	SetErrorInfo(0, pErrInfo);

	//Освобождаем интерфейсы
	pErrInfo->Release();
	pCrErrInfo->Release();

	return DISP_E_EXCEPTION;
}

STDMETHODIMP CSourceCodeControl::getVendors(BSTR *szVendorList)
{
	std::vector<std::string> items;
	_bstr_t vendor_list("");
	long lcurrent_item = 0;
	
	if(!m_scc.EnumVendors( items, lcurrent_item ))
		return RaiseError(0, "Can't enumerate SCC vendors", "");

	for(std::vector<std::string>::iterator it = items.begin(); 
		it != items.end(); ++it)
	{
		if(it != items.begin()) vendor_list += ";";
		vendor_list += (*it).c_str();
	}

	*szVendorList = vendor_list.copy();
	return S_OK;
}

STDMETHODIMP CSourceCodeControl::connect(BSTR szVendor, BSTR szUser, BSTR szPassword)
{
	_bstr_t vendor(szVendor);

	try {
		if(!m_scc.SelectVendor(vendor))
			return RaiseError(0, "Can't connect to the vendor", "REBUS::SCC::connect(1)");
	}
	catch( CSCCException e ) {
		return RaiseError(0, e.m_message.c_str(), e.m_source.c_str());
	}

	m_user.Assign(szUser);
	m_password.Assign(szPassword);

	return S_OK;
}

STDMETHODIMP CSourceCodeControl::getFiles(BSTR szProject, BSTR szLocal, BSTR szMask)
{
	if(!m_scc.SccInitialize(NULL, "REBUS.SCC")) {
		return RaiseError(0, std::string("Can't initialize vendor. ").append(m_scc.GetErrorText().c_str()).c_str(), 
			"REBUS::SCC::getFiles(1)");
	}

	TCHAR lpszProject[1024] = {0};
	strcpy(lpszProject, (char *) _bstr_t(szProject));

	TCHAR lpszAUXProject[1024] = {0}, lpszComment[1024] = {0};

	if(!m_scc.SccOpenProject(NULL, (char *) m_user, lpszProject, (char *) _bstr_t(szLocal),
			lpszAUXProject, lpszComment, 0)) 
	{
		return RaiseError(0, std::string("Can't open a project. ").append(m_scc.GetErrorText() .c_str()).c_str(), 
			"REBUS::SCC::getFiles(2)");
	}

	std::vector<_vssItemInfo> files_info;

	if(!m_scc.SccPopulateList(SCC_command_get, 0, NULL, files_info, NULL, 0)) 
	{
		m_scc.SccCloseProject();
		return RaiseError(0, std::string("Can't get file list. ").append(m_scc.GetErrorText() .c_str()).c_str(), 
			"REBUS::SCC::getFiles(3)");
	}

	// filter files using the mask
	class dos_mask 
	{
		template <typename I> bool _match(I finish, I source_it, I mask_it)	
		{
			bool bmatches = true;
			while(source_it != finish) 
			{
				if( *source_it == '.' && (*mask_it == '*' ? *(++mask_it) : *mask_it) != *source_it) {
					bmatches = false;
					break;
				}

				if( *mask_it != '*') 
				{
					if(*mask_it != '?' && 
						tolower(static_cast<unsigned char>(*source_it)) != 
						tolower(static_cast<unsigned char>(*mask_it)) ) 
					{
						bmatches = false;
						break;
					}
					mask_it++;
				}
				source_it++;
			}
			return bmatches;
		}
	public:
		bool match(std::string &source, std::string &mask) {
			return _match(source.end(), source.begin(), mask.begin()) &&
					_match(source.rend(), source.rbegin(), mask.rbegin());
		}
	} dos_mask_matcher;

	_bstr_t com_mask(szMask);
	std::string mask((char *) com_mask);
	std::vector<LPTSTR> files;

	for(std::vector<_vssItemInfo>::iterator it = files_info.begin(); it != files_info.end(); ++it)
	{
		if(dos_mask_matcher.match(it->path.substr(it->path.find_last_of("\\/") + 1), mask) ) 
		{
			files.push_back((char *) it->path.c_str());

			// remove local copy
			DeleteFile(it->path.c_str());
		}
	}

	// get local copies of files
	if(!m_scc.SccGetLatest(NULL, files, 0, NULL)) 
	{
		m_scc.SccCloseProject();
		return RaiseError(0, std::string("Can't get files. ").append(m_scc.GetErrorText() .c_str()).c_str(), 
			"REBUS::SCC::getFiles(4)");
	}

	// clear read only flag 
	DWORD dwFileAttributes = 0;
	for(std::vector<LPTSTR>::iterator itf = files.begin(); itf != files.end(); ++itf)
	{
		dwFileAttributes = GetFileAttributes(*itf);
		SetFileAttributes(*itf, dwFileAttributes & ~FILE_ATTRIBUTE_READONLY);
	}

	m_scc.SccCloseProject();

	return S_OK;
}
