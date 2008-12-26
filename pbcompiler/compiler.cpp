// compiler.cpp : Implementation of CCompiler

#include "stdafx.h"
#include "Pbcompiler.h"
#include "compiler.h"
#include "errortext.h"
#include "../common/pbl2xml.h"

#include <list>

typedef std::list<_compile_error> _c_error_list;

/////////////////////////////////////////////////////////////////////////////
// usefull functions
bool getFolder(_bstr_t &path, _bstr_t &folder)
{
	CString cs_path((char *) path);
	int ipos = cs_path.ReverseFind(_T('\\'));

	if(ipos < 1) return false;

	folder = (LPTSTR) (LPCSTR) cs_path.Left(ipos);
	return true;
}

_bstr_t getName(_bstr_t &path) 
{
	CString cs_path((char *) path);

	int ipos = cs_path.ReverseFind(_T('\\'));
	if(ipos > 0)
		cs_path = cs_path.Right(cs_path.GetLength() - ipos - 1);

	_bstr_t result = (LPTSTR) (LPCSTR) cs_path;
	return result;
}

_bstr_t getPbdName(_bstr_t &path) 
{
	_bstr_t &name = getName(path);
	CString cs_path((char *) name);

	cs_path = cs_path.Left(cs_path.GetLength() - 4) + ".pbd";

	_bstr_t result = ""; result = (LPTSTR) (LPCSTR) cs_path;
	return result;
}

_bstr_t getPbdPath(_bstr_t &path) 
{
	CString cs_path((char *) path);

	cs_path = cs_path.Left(cs_path.GetLength() - 4) + ".pbd";

	_bstr_t result = (LPTSTR) (LPCSTR) cs_path;
	return result;
}
/////////////////////////////////////////////////////////////////////////////
// CCompiler

STDMETHODIMP CCompiler::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IPBCompiler
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CCompiler::Init(BSTR szPbVersion, BSTR szBinPath)
{
	_bstr_t version( szPbVersion );
	if(!orca.Use((char *) version))
		return error(CString("Can't load ORCA library for ") + (char *) _bstr_t(szPbVersion));

	m_binpath = szBinPath;

	// initialization completed sucessfully
	m_bInit = true;

	return S_OK;
}

STDMETHODIMP CCompiler::SetLibraryList(BSTR szXMLLibList)
{
	m_liblist->async = false;
	m_liblist->loadXML( szXMLLibList );
		
	return S_OK;
}

void CALLBACK CompileErrorCallback(PPBORCA_COMPERR pErr, LPVOID lpData)
{
	_c_error_list *perrlist = (_c_error_list *) lpData;
	_compile_error err;

	err.ilevel = pErr->iLevel;
	err.szmsgnum = pErr->lpszMessageNumber;
	err.szmsgtxt = pErr->lpszMessageText;
	err.icolnum = pErr->iColumnNumber;
	err.ilinenum = pErr->iLineNumber;
	
	perrlist->insert(perrlist->end(), err);
};

void CCompiler::RegenerateObject(_bstr_t &path, _bstr_t &name, _bstr_t &type) throw()
{
	_c_error_list errlist;
	pborca_type nType = static_cast<pborca_type>(atoi((char *) type));

	int iCode = orca.CompileEntryRegenerate((char *) path, (char *) name,
		nType, CompileErrorCallback, &errlist);

	if(iCode != PBORCA_OK) {
		CString text = GetErrorText(iCode, path, name);

		for(_c_error_list::iterator &it = errlist.begin(); it != errlist.end(); ++it)
			text += GetErrorText(*it);

		throw text;
	}
}

void CCompiler::RegenerateLibrary(_bstr_t &name) throw()
{
	if(!InitializeSession()) { 
		log.write("Can't initialize ORCA session.");
		return;
	}

	CString cs_ext((char *) name); 
	cs_ext = cs_ext.Right(3);
	cs_ext.MakeLower();

	if( cs_ext == "pbd" )
	{
		// скомпилированные модули не регенерируем
		CString msg; msg.Format("skipped < %s >", (char *) name);
		out(msg);
	}
	else if (cs_ext == "pbl") 
	{
		// определим путь к файлу библиотеки
		MSXML2::IXMLDOMElementPtr lib_node = 
			m_liblist->documentElement->selectSingleNode("library[name/.='"+ name +"']");

		MSXML2::IXMLDOMElementPtr path_node = 
			lib_node->selectSingleNode("path");

		// населяем список объектов библиотеки
		CPbl2XML pbLibraryConverter(orca);
		pbLibraryConverter.Convert(lib_node, path_node->text);

		// выбираем объекты библиоетки
		//
		CString msg; msg.Format("regenerate < %s >", (char *) name);
		out(msg);

		MSXML2::IXMLDOMNodeListPtr childlist = 
			lib_node->selectNodes("childs/object[@type != \"1\"]");

		MSXML2::IXMLDOMElementPtr object;
		_bstr_t objname, type;

		// итерируем по объектам, регенерируем их
		for(int i = 0; i < childlist->length; i++)
		{
			object = childlist->item[i];
			objname = object->getAttribute("name");
			type = object->getAttribute("type");

			msg.Format("    %s...", (char *) objname);
			log.write(msg);

			RegenerateObject(path_node->text, objname, type);
		}	
	}
	else {
		CString msg; msg.Format("skipped < %s >", (char *) name);
		out(msg);
	}

	log.write(CString("Ok."));
}

void CCompiler::RegenerateAll()
{
	if(!InitializeSession()) { 
		log.write("Can't initialize ORCA session.");
		return;
	}

	MSXML2::IXMLDOMNodeListPtr &liblist = m_liblist->documentElement->childNodes;
	MSXML2::IXMLDOMElementPtr library, node;

	for(int i = 0; i < liblist->length; i++)
	{
		library = liblist->item[i];
		
		node = library->selectSingleNode("name");
		RegenerateLibrary(node->text);
		
		library = NULL;
	}

	liblist = NULL;
}

bool CCompiler::buildLibrary(_bstr_t &name)
{
	if(!InitializeSession()) { 
		log.write("initialization was not completed.");
		return S_FALSE;
	}

	CString cs_ext((char *) name); cs_ext = cs_ext.Right(3);
	cs_ext.MakeLower();

	MSXML2::IXMLDOMElementPtr path_node = 
		m_liblist->documentElement->selectSingleNode("library[name/.='"+ name +"']/path");
	_bstr_t path = path_node->text;

	if( cs_ext == "pbd" )
	{
		// готовые модули компоновать не нужно
		CString msg; msg.Format("copy < %s >", (char *) name);
		out(msg);

		// необходимо его скопировать в каталог сборки
		CString where; where.Format("%s\\%s", (char *) m_binpath,
			(char *) getPbdName(path));
			
		::DeleteFile(where);

		if(!CopyFile(getPbdPath(path), where, FALSE)) {
			CString error; error.Format(IDSI_COPYFILE1, getPbdPath(path));
			throw error;
		}
	}
	else if ( cs_ext == "pbl")
	{
		CString msg; msg.Format("build < %s >", (char *) name);
		out(msg);

		// определим пути к библиотеке и файлу ресурсов
		MSXML2::IXMLDOMElementPtr res_node = 
			m_liblist->documentElement->selectSingleNode("library[name/.='"+ name +"']/resource");
		_bstr_t resource = res_node->text;

		// для успешной компоновки библиотеки необходимо
		// задать текущую директорию, в которой располагаются ресурсы
		// (чтобы ресурсы находились)
		_bstr_t folder = "";

		if(resource != _bstr_t("") && !getFolder(resource, folder)) {
			CString msg; msg.Format("Bad resource file path: %s", (char *) resource);
			throw msg;
		}

		if(folder != _bstr_t("") && !::SetCurrentDirectory((char *) folder)) 
		{
			CString msg; msg.Format("Bad resource folder path: %s", (char *) folder);
			throw msg;
		}

		// компонуем библиотеку
		int result = orca.DynamicLibraryCreate(
			(char *) path, (char *) resource, PBORCA_P_CODE);

		if(result != PBORCA_OK) {
			CString error;

			switch(result)
			{
			case PBORCA_INVALIDPARMS:
				error.LoadString(IDSI_BUILD5);
				throw error;
			case PBORCA_BADLIBRARY:
				error.Format(IDSI_BUILD6, (char *) path);
				throw error;
			case PBORCA_PBDCREATERROR:
				error.Format(IDSI_BUILD7, (char *) path);
				throw error;
			}
		}

		// поскольку результат компоновки распологается рядом
		// с исходной библиотекой, необходимо его скопировать в каталог
		// сборки
		CString where; where.Format("%s\\%s", (char *) m_binpath,
			(char *) getPbdName(path));
			
		::DeleteFile(where);

		if(!MoveFile(getPbdPath(path), where)) {
			CString error; error.Format(IDSI_COPYFILE1, getPbdPath(path));
			throw error;
		}
	}
	else
	{
		CString msg; msg.Format("don't know how to build < %s >", (char *) name);
		out(msg);
	}


	return true;
}

bool CCompiler::buildAll()
{
	if(!InitializeSession()) { 
		log.write("Can't initialize ORCA session.");
		return S_FALSE;
	}

	// убедимся что каталог для сборки подготовлен
	if(::GetFileAttributes((char *) m_binpath) == 0xffffffff)
		::CreateDirectory((char *) m_binpath, NULL);

	// предварительно создадим каталог для сборки
	// если этого небыло сделано раньше

	MSXML2::IXMLDOMNodeListPtr &liblist = m_liblist->documentElement->childNodes;
	MSXML2::IXMLDOMElementPtr library, node;

	for(int i = 0; i < liblist->length; i++)
	{
		library = liblist->item[i];

		node = library->selectSingleNode("name");
		
		// компонуются только те библиотеки, которые
		// не включаются в исполняемый модуль
		if(!IsLinkedLibrary(node->text)) buildLibrary(node->text);
		
		library = NULL;
	}

	// компонуем исполняемый модуль
	buildExe();

	liblist = NULL;
	return true;
}

void CALLBACK LinkerErrorCallback(PPBORCA_LINKERR pErr, LPVOID lpData) 
{
	_linker_errors &errors = (*(_linker_errors *) lpData);

	errors.insert(errors.end(), pErr->lpszMessageText);
}

bool CCompiler::buildExe()
{
	if(!InitializeSession()) { 
		log.write("initialization was not completed.");
		return S_FALSE;
	}

	// убедимся что каталог для сборки подготовлен
	if(::GetFileAttributes((char *) m_binpath) == 0xffffffff)
		::CreateDirectory((char *) m_binpath, NULL);

	// для успешной компоновки библиотеки необходимо
	// задать текущую директорию, в которой располагаются ресурсы
	// (чтобы ресурсы находились)
	_bstr_t folder = "";

	if(m_appresource != _bstr_t("") && !getFolder(m_appresource, folder)) {
		CString msg; msg.Format("Bad resource file path: %s", (char *) m_appresource);
		throw msg;
	}

	if(folder != _bstr_t("") && !::SetCurrentDirectory((char *) folder)) 
	{
		CString msg; msg.Format("Bad resource folder path: %s", (char *) folder);
		throw msg;
	}

	// составим массив масок, определяющих вхождение библиотеки
	// в исполняемый модуль
	//
	MSXML2::IXMLDOMNodeListPtr &liblist = m_liblist->documentElement->childNodes;
	MSXML2::IXMLDOMElementPtr library, node;

	int nLength = liblist->length;
	int *pbdMask = new int[nLength];

	for( int i = 0; i < nLength; i++) 
	{
		library = liblist->item[i];

		node = library->selectSingleNode("name");
		pbdMask[i] = IsLinkedLibrary(node->text) ? 0 : 1; 

		library = NULL;
	}

	// удалим старый исполняемый модуль, иначе компоновка не свершится
	::DeleteFile((char *) m_exepath);

	_linker_errors errors;

	// компонуем исполняемый модуль
	out("link executable");

	int result = orca.ExecutableCreate((char *) m_exepath, (char *) m_iconpath,
		(char *) m_appresource,	LinkerErrorCallback, &errors, pbdMask,
	    nLength, PBORCA_P_CODE);

	// если есть ошибки, то заканичавем на этом
	CString longmessage;

	for(std::vector<CString>::iterator it = errors.begin();
			it != errors.end(); ++it ) longmessage += *it;
	
	if(!longmessage.IsEmpty()) throw longmessage;

	// проверка на неверные параметры и состояние окружения
	if(result != PBORCA_OK) 
	{
		CString error = getDisplayError(result);

		if(error != "") throw error;
	}

	return true;
}

CString CCompiler::getDisplayError(int iCode)
{
	CString error;

	switch(iCode)
	{
	case PBORCA_INVALIDPARMS:
		error.LoadString(IDSI_BUILD9);
		break;
	case PBORCA_LIBLISTNOTSET:
		error.LoadString(IDSI_BUILD10);
		break;
	case PBORCA_LINKERROR:
		error.LoadString(IDSI_BUILD11);
		break;
	case PBORCA_CURRAPPLNOTSET:
		error.LoadString(IDSI_BUILD12);
		break;
	}

	return error;
}

bool CCompiler::Migrate_()
{
	if(!InitializeSession()) { 
		log.write("initialization was not completed.");
		return S_FALSE;
	}

	int iCode;
	_c_error_list errors;

	out("migrate project");

	iCode = orca.ApplicationRebuild(PBORCA_MIGRATE, 
									CompileErrorCallback, &errors);

	if(iCode != PBORCA_OK) 
	{
		CString message = getDisplayError(iCode);

		for(_c_error_list::iterator it = errors.begin();
				it != errors.end(); ++it ) message += GetErrorText(*it);

		if(message != "") throw message;
	}

	return true;
}

bool CCompiler::FullRebuild_()
{
	if(!InitializeSession()) { 
		log.write("initialization was not completed.");
		return S_FALSE;
	}

	int iCode;
	_c_error_list errors;

	out("full rebuild of the project");

	iCode = orca.ApplicationRebuild(PBORCA_FULL_REBUILD, 
									CompileErrorCallback, &errors);

	if(iCode != PBORCA_OK) 
	{
		CString message = getDisplayError(iCode);

		for(_c_error_list::iterator it = errors.begin(); it != errors.end(); ++it ) 
		{	
			message += "\r\n";
			if(it->ilevel > 3) {
				message += "<b>";
				message += GetErrorText(*it);
				message += "</b>";
			}
			else {
				message += GetErrorText(*it);
			}
		}

		if(message != "") throw message;
	}

	m_bSessionReady = false;

	return true;
}

////////////////////////////////////////////////////////////////////
// INTERFACE functions declaration
//
STDMETHODIMP CCompiler::Regenerate()
{
	if(!m_regInfo->IsRegistered())
		return error(gpszNotRegisteredText);

	try {
		log.write("full regeneration.");

		if(!m_bInit) {
			log.write("initialization was not completed.");
			return S_FALSE;
		}

		if(!InitializeSession()) {
			log.write("Can't initialize ORCA session.");
			return S_FALSE;
		}

		RegenerateAll();
	}
	catch(CString errmsg) 
	{
		return error(errmsg);
	}
	catch(...) {
		return error("unknown exception.");
	}

	return S_OK;
}

STDMETHODIMP CCompiler::RegenerateLibrary(BSTR libname)
{ 
	if(!m_regInfo->IsRegistered())
		return error(gpszNotRegisteredText);

	_bstr_t name(libname);

	try {
		CString msg; msg.Format("Single library regeneration");
		log.write(msg);

		if(!m_bInit) {
			return error("initialization was not completed.");
		}

		if(!InitializeSession()) {
			return error("Can't initialize ORCA session.");
		}

		RegenerateLibrary(name);
	}
	catch(CString errmsg) 
	{
		return error(errmsg);
	}
	catch(...) {
		return error("unknown exception.");
	}

	return S_OK;
}

STDMETHODIMP CCompiler::BuildLibrary(BSTR libname)
{
	if(!m_regInfo->IsRegistered())
		return error(gpszNotRegisteredText);

	_bstr_t name(libname);

	try {
		CString msg; msg.Format("Single library build: %s", (char *) name);
		log.write(msg);

		if(!m_bInit) {
			return error("initialization was not completed.");
		}

		if(!InitializeSession()) {
			return error("Can't initialize ORCA session.");
		}

		if(IsLinkedLibrary(name)) {
			buildExe();
		} else {
			buildLibrary(name);
		}
	}
	catch(CString errmsg) 
	{
		return error(errmsg);
	}
	catch(...) {
		return error("unknown exception.");
	}

	return S_OK;
}

STDMETHODIMP CCompiler::Build()
{
	if(!m_regInfo->IsRegistered())
		return error(gpszNotRegisteredText);

	try {
		log.write("full build");

		if(!m_bInit) {
			return error("initialization was not completed.");
		}

		if(!InitializeSession()) {
			return error("Can't initialize ORCA session.");
		}

		buildAll();
	}
	catch(CString errmsg) 
	{
		return error(errmsg);
	}
	catch(...) {
		return error("unknown exception.");
	}

	return S_OK;
}

bool CCompiler::InitializeSession( tSessionKind kind )
{
	if(m_bSessionReady) return true;

	if(orca.m_hSession) orca.SessionClose();
	orca.SessionOpen();

	CString msg;

	MSXML2::IXMLDOMNodeListPtr &liblist = m_liblist->documentElement->childNodes;
	liblist_.clear();
 
	LPSTR* sessionliblist = new LPSTR[(int) liblist->length];
		
	_bstr_t type, path;
	MSXML2::IXMLDOMElementPtr library, node;

	log.write("initialize orca session.");

	CString text;
	text.Format("libraries num in the list: %d", (long) liblist->length);
	log.write(text);

	for(int i = 0; i < liblist->length; i++)
	{
		library = liblist->item[i];

		node = library->selectSingleNode("path");
		path = node->text;

		CString cs_ext ((char *) path);
		cs_ext.MakeLower(); 
		cs_ext = cs_ext.Right(3);

		if(GetFileAttributes((char *) path) == 0xffffffff) 
		{
			log.write((char *) path);
		}

		switch (kind)
		{
		case SESSION_REGENERATE:
			if (cs_ext == "pbl")
				liblist_.push_back(std::string((char *) path));
			break;

		case SESSION_BUILD:
			liblist_.push_back(std::string((char *) path));
			break;
		}

		library = NULL;
	}

	i = 0;
	std::vector<std::string>::iterator it;
	for(it = liblist_.begin(); it != liblist_.end(); ++it, i++)
	{
		sessionliblist[i] = (char *) it->c_str();
	}

	int iCode = orca.SessionSetLibraryList(sessionliblist, i);

	delete[] sessionliblist;

	// проверим результат опреации
	if(iCode!=PBORCA_OK)
	{
		out("ORCA: Can't set list of libraries:");

		CString message;
		if(iCode==PBORCA_INVALIDPARMS) {
			message.LoadString(IDSI_PBORCA3);
		}
		
		if(iCode==PBORCA_BADLIBRARY) {
			message.LoadString(IDSI_PBORCA4);
		}

		out(message);

		return false;			
	}

	iCode = orca.SessionSetCurrentAppl((char *) m_applibrary, (char *) m_appobject);

	if(iCode!=PBORCA_OK)
	{
		out("ORCA: Can't set current application.");

		switch(iCode)
		{
		case PBORCA_INVALIDPARMS:
			msg.LoadString(IDSI_PBORCA3);
			break;
		case PBORCA_DUPOPERATION:
			msg.LoadString(IDSI_PBORCA5);
			break;
		case PBORCA_OBJNOTFOUND:
			msg.LoadString(IDSI_PBORCA6);
			break;
		case PBORCA_BADLIBRARY:
			msg.LoadString(IDSI_PBORCA7);
			break;
		case PBORCA_LIBLISTNOTSET:
			msg.LoadString(IDSI_PBORCA8);
			break;
		case PBORCA_LIBNOTINLIST:
			msg.LoadString(IDSI_PBORCA9);
			break;
		}

		out(msg);

		return false;
	}

	m_bSessionReady = true;
	return true;
}

///////////////////////////////////////////////////////////////////////////
//
void CCompiler::out(LPCSTR what)
{
	Fire_log(_bstr_t(what));
	log.write(what);
}

void CCompiler::out(CString &what)
{
	out((LPTSTR) (LPCSTR) what);
}

HRESULT CCompiler::error(LPCSTR what)
{
	Fire_fatalError(_bstr_t(what));
	log.write(what);

	return RaiseError(1, _bstr_t(what), "", "", 0);
}

HRESULT CCompiler::error(CString &what)
{
	return error((LPTSTR) (LPCSTR) what);
}

HRESULT CCompiler::RaiseError(double number, _bstr_t description, 
									_bstr_t source, _bstr_t helpFile, 
									unsigned long helpcontext)
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
		pCrErrInfo->SetHelpContext(helpcontext);
		pCrErrInfo->SetHelpFile(helpFile);
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

STDMETHODIMP CCompiler::Migrate()
{
	if(!m_regInfo->IsRegistered())
		return error(gpszNotRegisteredText);

	try {
		log.write("migrate");

		if(!m_bInit) {
			return error("initialization was not completed.");
		}

		if(!InitializeSession()) {
			return error("Can't initialize ORCA session.");
		}

		Migrate_();
	}
	catch(CString errmsg) 
	{
		return error(errmsg);
	}
	catch(...) {
		return error("unknown exception.");
	}

	return S_OK;
}

STDMETHODIMP CCompiler::FullRebuild()
{
	if(!m_regInfo->IsRegistered())
		return error(gpszNotRegisteredText);

	try {
		log.write("full rebuild");

		if(!m_bInit) {
			return error("initialization was not completed.");
		}

		if(!InitializeSession()) {
			return error("Can't initialize ORCA session.");
		}

		FullRebuild_();
	}
	catch(CString errmsg) 
	{
		return error(errmsg);
	}
	catch(...) {
		return error("unknown exception.");
	}

	return S_OK;
}

STDMETHODIMP CCompiler::SetLinkedLibrary(BSTR szLibList)
{
	linkedlist_.push_back( (char *) _bstr_t(szLibList) );

	return S_OK;
}

bool CCompiler::IsLinkedLibrary(_bstr_t name)
{
	std::vector<std::string>::iterator it;
	for(it = linkedlist_.begin(); it != linkedlist_.end(); ++it)
	{
		if(name == _bstr_t(it->c_str())) return true;
	}
	return false;
}

STDMETHODIMP CCompiler::SetApplicationLibrary(BSTR szAppLibrary)
{
	_bstr_t applibrary(szAppLibrary);

	MSXML2::IXMLDOMElementPtr node = 
		m_liblist->documentElement->selectSingleNode("library[name/.='"+ applibrary +"']");
	MSXML2::IXMLDOMElementPtr node_path = node->selectSingleNode("path"),
		node_resource = node->selectSingleNode("resource");
	
	m_applibrary = node_path->text;
	m_appresource = node_resource->text;

	return S_OK;
}

STDMETHODIMP CCompiler::SetExeName(BSTR szExeName)
{
	m_exepath = m_binpath + "\\" + szExeName;

	return S_OK;
}

STDMETHODIMP CCompiler::SetIconFile(BSTR szIconFile)
{
	m_iconpath = szIconFile;

	return S_OK;
}

STDMETHODIMP CCompiler::SetApplicationObject(BSTR szApplicationObject)
{
	m_appobject = szApplicationObject;

	return S_OK;
}

LPCTSTR gpszProductSalt1 = "{E7AE23D2-12AE-447a-99A0-0972A8EFA7C9}";

STDMETHODIMP CCompiler::SetApplicationResourceFile(BSTR szApplicationResourceFile)
{
	m_appresource = szApplicationResourceFile;

	return S_OK;
}
