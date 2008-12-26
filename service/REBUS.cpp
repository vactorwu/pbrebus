// REBUS.cpp: implementation of the CREBUS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "REBUS.h"

// import REBUS EMailer component
#import "../email/releaseminsize/rebusmail.dll" named_guids no_namespace

LPCTSTR gpszProductSalt2 = "{B765C842-66B5-4374-A1AE-2CF623939E5B}";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CREBUS::CREBUS()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
}

CREBUS::~CREBUS()
{
	if(m_regInfo) delete m_regInfo;
	if(m_rebus != NULL)	m_rebus.Release();

	CoUninitialize();
}


void CREBUS::init(CLogFile &log)
{
	m_log = log;

	CString path;
	registry.GetMachineEntry("REBUS", "MainFolder", path);
	m_rootpath = (LPCTSTR) path;

	m_regInfo = new CRegistrationInfo;

	m_rebus.CreateInstance(RebusTypeLib::CLSID_ScriptletCoClass);
	m_rebus->init(_variant_t());

	m_regInfo->setSatelliteCLSID("{77765AE8-6757-4ad0-8741-FCC99DE0E0A4}");
}

struct ThreadBuildProjectData 
{
	CREBUS *pRebus;
	_bstr_t project_id;

	ThreadBuildProjectData(CREBUS *rebus, _bstr_t& project) :
		pRebus(rebus), project_id(project) {}
};

void CREBUS::buildProject(_bstr_t id)
{
	// run build project thread, which will be looking for 
	// build process state
	DWORD dwThreadId = 0;

	HANDLE hthread = CreateThread(NULL, 0, ThreadBuildProject, 
		(LPVOID) new ThreadBuildProjectData(this, id), 0, &dwThreadId);

	// append the thread into the collection of project builds
	m_buildmap.insert( tMapPair(id, tThreadBuildHandle(hthread, dwThreadId)) );
}

DWORD WINAPI ThreadBuildProject(LPVOID lpData)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ThreadBuildProjectData *pData = (ThreadBuildProjectData *) lpData;

	try {
		// get particular project
		ProjectTypeLib::Automation2Ptr project = 
			pData->pRebus->getProject(atol((char *) pData->project_id));

		// start project build
		project->build(_variant_t());

		// check the build process state
		while(true) 
		{
			Sleep(1000);

			try {
				if(! ((bool) project->isalive(_variant_t()))) 
				{
					// if the build process doesn't exist
					// report on errors and finish the thread
					_bstr_t error = project->errorText(_variant_t());

					if(error.length() > 0)
					{
						try {
							MSXML2::IXMLDOMDocumentPtr config = 
								pData->pRebus->config();

							// get the former of the e-mail
							_bstr_t from = config->selectSingleNode("/rebus/settings/email")->text;

							// get the recipients of the e-mail notification
							MSXML2::IXMLDOMDocumentPtr project_config = project->Getconfig();
							_bstr_t to = project_config->selectSingleNode("/project/email")->text;

							if(from.length() > 0 && to.length() > 0) 
							{
								// if the former and recipients are assigned then 
								// send an e-mail notification

								IEMailerPtr smtp;
								smtp.CreateInstance(CLSID_EMailer);
								
								smtp->SetFrom(from);
								smtp->SetTo(to);
								smtp->SetSubject("REBUS: Report on compilation error");

								_bstr_t body("The error has occurred during the project compilation:\r\n\r\n");
								
								body += "Project: " + _bstr_t(project->Getproduct());
								body += "\r\n\r\n";
								body += error;

								body += "There is the build log:";

								body += "\r\n\r\n";

								body += _bstr_t(project->output(_variant_t()));
								smtp->SetBody(body);

								smtp->Send();
							}
						}
						catch(_com_error e) {
							pData->pRebus->m_log.write("Can't send e-mail notification on compilation error. ", e);
						}
					}
					break;
				}
			}
			catch( _com_error e )
			{
				pData->pRebus->m_log.write("Can't get the state of the project build. ", e);
			}
		}
	}
	catch( _com_error e )
	{
		pData->pRebus->m_log.write("Can't get project to build. ", e);
	}

	// remove from collection of project builds
	pData->pRebus->m_buildmap.erase( pData->project_id );

	// free memory
	delete pData;

	CoUninitialize();

	return 1;
}

void CREBUS::changeBuildPriority(_bstr_t project_id, int direction)
{
	ProjectTypeLib::Automation2Ptr project = 
		rebus.getProject(atol((char *) project_id));
	
	_bstr_t process_id = project->Getprocess_id();

	HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION, 
		FALSE, atol(process_id));

	if(hProcess)
	{
		const int iClasses = 4;
		DWORD dwPriorities[iClasses] = { IDLE_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, 
										 NORMAL_PRIORITY_CLASS, ABOVE_NORMAL_PRIORITY_CLASS};

		DWORD dwPriorityClass = GetPriorityClass(hProcess);

		for(int i = 0; i < iClasses; i++) {
			if(dwPriorities[i] == dwPriorityClass) {
				if(i + direction >=0 && i + direction < iClasses)
				{
					dwPriorityClass = dwPriorities[i + direction];
					break;
				}
			}
		}

		SetPriorityClass(hProcess, dwPriorityClass);
		CloseHandle(hProcess);
	}
}

_bstr_t CREBUS::getBuildPriority(_bstr_t project_id)
{
	ProjectTypeLib::Automation2Ptr project = 
		rebus.getProject(atol((char *) project_id));
	
	_bstr_t process_id = project->Getprocess_id();

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, atol(process_id));
	if(hProcess) 
	{
		DWORD dwPriorityClass = GetPriorityClass(hProcess);
		CloseHandle(hProcess);

		switch(dwPriorityClass)
		{
		case ABOVE_NORMAL_PRIORITY_CLASS:
			return "High";
		case NORMAL_PRIORITY_CLASS:
			return "Normal";
		case BELOW_NORMAL_PRIORITY_CLASS:
			return "Low";
		case IDLE_PRIORITY_CLASS:
			return "Idle";
		}
	}
	return "";
}

long CREBUS::getProjectsCount()
{
	return m_rebus->getProjectCount(_variant_t());
}

ProjectTypeLib::Automation2Ptr
CREBUS::getProject(long index)
{
	return m_rebus->getProject( _variant_t(index, VT_I4) );
}

MSXML2::IXMLDOMDocumentPtr 
CREBUS::getProjectConfig(long index)
{
	ProjectTypeLib::Automation2Ptr project = 
		m_rebus->getProject( _variant_t(index, VT_I4) );
	return project->Getconfig();
}

long CREBUS::CreateProject( _bstr_t codename, _bstr_t path, _bstr_t product,
							_bstr_t description, _bstr_t stemplate)
{
	return m_rebus->createProject(codename, path, product, 
		description, stemplate, _bstr_t(""));
}

long CREBUS::getTemplateCount()
{
	return m_rebus->getTemplateCount(_variant_t());
}

_bstr_t CREBUS::getTemplate(long index)
{
	return m_rebus->getTemplate(_variant_t(index));
}

void CREBUS::removeProject(_bstr_t codename)
{
	m_rebus->removeProject(codename);
}

MSXML2::IXMLDOMDocumentPtr 
CREBUS::config()
{
	return m_rebus->Getconfig();
}

void CREBUS::saveConfig()
{
	m_rebus->saveConfig(_variant_t());
}

_bstr_t CREBUS::getPlugin(long index)
{
	return m_rebus->getPlugin(_variant_t(index));
}

long CREBUS::getPluginCount()
{
	return m_rebus->getPluginCount(_variant_t());
}

MSXML2::IXMLDOMDocumentPtr
CREBUS::getPluginParmsDef(_bstr_t coclass)
{
	PluginTypeLib::PluginPtr plugin;
	plugin.CreateInstance((char *) coclass);

	MSXML2::IXMLDOMDocumentPtr xml;
	xml.CreateInstance(MSXML2::CLSID_DOMDocument26);
	
	try {
		xml->loadXML( _bstr_t(plugin->getParmsDef(_variant_t())) );
	}
	catch(_com_error e) {
		_bstr_t text = e.Description();
	}

	return xml;
}

_bstr_t CREBUS::getPage(LPCTSTR name)
{
	_bstr_t path = getRootPath();
	path += "\\pages\\";
	path += name;
	path += ".esp";

	return path;
}

_bstr_t CREBUS::getRootPath()
{
	return m_rootpath;
}
