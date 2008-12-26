// EMailer.cpp : Implementation of CEMailer
#include "stdafx.h"
#include "Rebusmail.h"
#include "EMailer.h"
#include "../common/stringex.h"

/////////////////////////////////////////////////////////////////////////////
// CEMailer

STDMETHODIMP CEMailer::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IEMailer
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CEMailer::RaiseError( double number, _bstr_t description, 
										_bstr_t source )
{
	ICreateErrorInfo *pCrErrInfo;
	HRESULT hr;
	HRESULT res;
	bool succ = true;
	//¦¨õþñ¨ð÷þòvòðõü ªøÿ úþôð þ°øñúø
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

	//¦þûº¢ðõü øýªõ¨¯õù¸ ôû  ¸þ÷ôðýø  øý¯þ¨üð¡øø þ þ°øñúõ
	hr = CreateErrorInfo(&pCrErrInfo);
	if (FAILED(hr))
	{
		return hr;
	}
	//L¸ªðýðòûøòðõü ÿð¨ðüõª¨v þ°øñúø
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
	//¦þûº¢ðõü øýªõ¨¯õù¸ ôû  º¸ªðýþòúø ÷ýð¢õýøù þ°øñþú
	hr = pCrErrInfo->QueryInterface(IID_IErrorInfo, (LPVOID FAR*)&pErrInfo);

	if (FAILED(hr))
	{
		return hr;
	}
	//L¸ªðýðòûøòðõª øý¯þ¨üð¡ø¦ þñ þ°øñúõ
	SetErrorInfo(0, pErrInfo);

	//+¸òþñþöôðõü øýªõ¨¯õù¸v
	pErrInfo->Release();
	pCrErrInfo->Release();

	return DISP_E_EXCEPTION;
}

STDMETHODIMP CEMailer::SetFrom(BSTR szFromAddress)
{
	CString from_address(szFromAddress);
	CString address, name = from_address.Left(from_address.Find("<"));
	name.TrimRight();

	if(name.GetLength() > 0) 
	{
		address = from_address.Right(from_address.GetLength() - from_address.Find("<") - 1);
		address = address.Left(address.Find(">"));

		CSMTPAddress from(name, address);
		m_mess.m_From = from;
	}
	else {
		CSMTPAddress from(from_address);
		m_mess.m_From = from;
	}

	return S_OK;
}

STDMETHODIMP CEMailer::SetTo(BSTR szToAddress)
{
	CString to_address(szToAddress);

	std::string source(to_address);
	std::vector<string> items;
	stringex::split(source, items, ",;");

	for(std::vector<string>::iterator it = items.begin(); it != items.end(); ++it)
	{
		to_address = it->c_str();
		CString address, name = to_address.Left(to_address.Find("<"));
		name.TrimRight();

		if(name.GetLength() > 0) 
		{
			address = to_address.Right(to_address.GetLength() - to_address.Find("<") - 1);
			address = address.Left(address.Find(">"));

			CSMTPAddress to(name, address);
			m_mess.AddRecipient(to);
		}
		else {
			CSMTPAddress to(to_address);
			m_mess.AddRecipient(to);
		}
	}

	return S_OK;
}

STDMETHODIMP CEMailer::SetSubject(BSTR szSubject)
{
	CString subject_string(szSubject);
	m_mess.m_sSubject = subject_string;

	return S_OK;
}

STDMETHODIMP CEMailer::SetBody(BSTR szBody)
{
	m_mess.AddBody(CString(szBody));

	return S_OK;
}

STDMETHODIMP CEMailer::Send()
{
	// extract smtp server address
	std::string source(m_mess.m_From.m_sEmailAddress);
	std::vector<string> items;
	stringex::split(source, items, "@");

	if(!m_conn.Connect((items.end() - 1)->c_str())) {
		_bstr_t text("Can't connect to the SMTP server: ");
		text += (LPCSTR) m_conn.GetLastCommandResponse();
		return RaiseError(0, text, "");
	}

	if(!m_conn.SendMessage(m_mess)) {
		_bstr_t text("Can't send message: ");
		text += (LPCSTR) m_conn.GetLastCommandResponse();
		return RaiseError(0, text, "");
	}

	m_conn.Disconnect();

	return S_OK;
}