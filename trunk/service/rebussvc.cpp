// asescheduler.cpp : Defines the entry point for the application.
//
#include "stdafx.h"

#include "eventlog.h"
#include "rebus.h"
#include "scheduledtask.h"

#define SERVICESTATUS_IMPL
#include "servicestatus.h"

#include "webserver.h"

// общие объекты
CServiceStatus	g_ssThis;
// планировщик задач
typedef CScheduler<CLogFile> CSchedulerService;
// название сервиса
TCHAR g_szServiceName[] = TEXT("REBUS Release Builder Service");

// реестр
CRegistry registry("PBDevEx");
// сервер приложения
CREBUS rebus;

//----------------------------------------------------------------------------------------------------------------
DWORD WINAPI SchedulerServiceLoopThread(LPVOID lpv);

// цикл выполнения задач
template <class CSchedulerLog>
class CSchedulerLoop
{
	friend DWORD WINAPI SchedulerServiceLoopThread(LPVOID lpv);

	CScheduler<CSchedulerLog> &m_scheduler;
	CServiceStatus &m_service;

	HANDLE hEvent;

public:
	bool running;

public:
	CSchedulerLoop(CScheduler<CSchedulerLog> &scheduler, 
		CServiceStatus &service) : m_scheduler(scheduler), 
		running(true), m_service(service)
	{
		hEvent = CreateEvent(NULL, TRUE, FALSE, _T("{3A5CB6F5-73BD-471d-ADF2-3492362ABF22}"));
	}

	////////////////////////////////////////////////////
	// выполняются в потоке-обработчике событий сервиса
	void start() 
	{
		running = true;
		ResetEvent(hEvent);
	}

	void stop() 
	{
		running = false;
		SetEvent(hEvent);
	}
	////////////////////////////////////////////////////

	////////////////////////////////////////////////////
	// выполняется в основном потоке сервиса
	void loop() 
	{
		//m_scheduler.m_log.write("in the loop");
		DWORD dwResult;

		if(running) 
		{
			for( int i = 0; i < m_scheduler.tasks.size(); i++)
			{
				m_scheduler.execute( i );
			}
		}

		dwResult = WaitForSingleObject(hEvent, 20000);
	}
	////////////////////////////////////////////////////
};
typedef CSchedulerLoop<CLogFile> CSchedulerServiceLoop;

//------------------------------------------------------------------------------------------------------------------

// обрабочик управляющих команд сервиса
//
DWORD WINAPI ServiceHandlerEx(DWORD dwControl, DWORD dwEventType, 
   PVOID pvEventData, PVOID pvContext) 
{

   DWORD dwReturn = ERROR_CALL_NOT_IMPLEMENTED;
   CSchedulerServiceLoop *ploop = (CSchedulerServiceLoop *) pvContext;

   switch (dwControl) 
   {
   case SERVICE_CONTROL_STOP:
   case SERVICE_CONTROL_SHUTDOWN:
      g_ssThis.SetUltimateState(SERVICE_STOPPED, 2000);
      ploop->stop();
      break;

   case SERVICE_CONTROL_PAUSE:
      g_ssThis.SetUltimateState(SERVICE_PAUSED, 2000);
      ploop->stop();
      break;

   case SERVICE_CONTROL_CONTINUE:
      g_ssThis.SetUltimateState(SERVICE_RUNNING, 2000);
      ploop->start();
      break;

   case SERVICE_CONTROL_INTERROGATE:
      g_ssThis.ReportStatus();
      break;

   default:
	   return dwReturn;
   }

   return(NO_ERROR);
}

//------------------------------------------------------------------------------------------------------------------

// дополнительный поток сервиса для обслуживания CSchedulerServiceLoop
DWORD WINAPI SchedulerServiceLoopThread(LPVOID lpv)
{
	CSchedulerServiceLoop *ploop = (CSchedulerServiceLoop *)lpv;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	ploop->m_scheduler.m_log.write("schedule server is started");

    while (g_ssThis != SERVICE_STOPPED)
	{
		ploop->loop();
	}

	ploop->m_scheduler.m_log.write("schedule server is stopped");
	//while(ploop->running) ploop->loop();

	CoUninitialize();

	return 1;
}

//------------------------------------------------------------------------------------------------------------------

// основной поток сервиса
//
void WINAPI ServiceMain(DWORD dwArgc, PTSTR* pszArgv) 
{
   CLogFile log;
   log.Init(GetModuleHandle(NULL));

	try {
		rebus.init(log);
	}
	catch(_com_error e) 
	{
		log.write("Can't initialize REBUS.Core", e);
	}

   rebus.m_regInfo->CheckProductRegistration();

   // сервис выполнения заданий по расписанию
   CSchedulerService scheduler(log);
   // цикл выполнения задач
   CSchedulerServiceLoop loop( scheduler, g_ssThis );
   // Web server
   CWebServer<CLogFile> w3c(log);

   // инициализируем обработчик сервиса
   g_ssThis.Initialize(g_szServiceName, ServiceHandlerEx, (PVOID) &loop, TRUE);
   g_ssThis.AcceptControls(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE);
   
   // запускаем дополнительный поток для планировщика задач
   DWORD dwThreadId = 0;
   HANDLE hthread = CreateThread(NULL, 0, SchedulerServiceLoopThread, (LPVOID) &loop, 0, &dwThreadId);

   do {
      if (g_ssThis != SERVICE_STOPPED) 
	  {
		  w3c.loop();

		  g_ssThis.ReportUltimateState();
      }
   } while (g_ssThis != SERVICE_STOPPED);

   w3c.stop();

   WaitForSingleObject(hthread, INFINITE);
   CloseHandle(hthread);
}

//------------------------------------------------------------------------------------------------------------------

void InstallService() {

   // Open the SCM on this machine.
   SC_HANDLE hSCM = 
      OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

   // Get our full pathname
   TCHAR szModulePathname[_MAX_PATH * 2];
   GetModuleFileName(NULL, szModulePathname, chDIMOF(szModulePathname));

   // Append the switch that causes the process to run as a service.
   lstrcat(szModulePathname, TEXT(" /service"));   

   // Add this service to the SCM's database.
   SC_HANDLE hService = 
      CreateService(hSCM, g_szServiceName, g_szServiceName,
         SERVICE_CHANGE_CONFIG, SERVICE_WIN32_OWN_PROCESS, 
         SERVICE_AUTO_START, SERVICE_ERROR_IGNORE,
         szModulePathname, NULL, NULL, NULL, NULL, NULL);

   if(hService) {
	   SERVICE_DESCRIPTION sd = { 
		  TEXT("REBUS Release Builder Service, PBDevEx Software")
	   };
	   ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sd);

	   CloseServiceHandle(hService);
   }
   CloseServiceHandle(hSCM);
}

//------------------------------------------------------------------------------------------------------------------

void RemoveService() {

   // Open the SCM on this machine.
   SC_HANDLE hSCM = 
      OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);

   // Open this service for DELETE access
   SC_HANDLE hService = 
      OpenService(hSCM, g_szServiceName, DELETE);

   // Remove this service from the SCM's database.
   DeleteService(hService);

   CloseServiceHandle(hService);
   CloseServiceHandle(hSCM);
}

//------------------------------------------------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
   int nArgc = __argc;
#ifdef UNICODE
   PCTSTR *ppArgv = (PCTSTR*) CommandLineToArgvW(GetCommandLine(), &nArgc);
#else
   PCTSTR *ppArgv = (PCTSTR*) __argv;
#endif

	registry.SetOrganisation("PBDevEx");

   if (nArgc < 2) {
      MessageBox(NULL, 
         TEXT("REBUS Release Builder Service\n\n")
         TEXT("Usage: rebussvc.exe [/install] [/remove] [/debug] ")
         TEXT("[/service]\n")
         TEXT("   /install\t\tInstalls the service in the SCM's database.\n")
         TEXT("   /remove\tRemoves the service from the SCM's database.\n"),
         g_szServiceName, MB_OK);
   } else {
      for (int i = 1; i < nArgc; i++) {
         if ((ppArgv[i][0] == TEXT('-')) || (ppArgv[i][0] == TEXT('/'))) {
            // Command line switch
            if (lstrcmpi(&ppArgv[i][1], TEXT("install")) == 0) 
               InstallService();

            if (lstrcmpi(&ppArgv[i][1], TEXT("remove"))  == 0)
               RemoveService();

            if (lstrcmpi(&ppArgv[i][1], TEXT("debug"))   == 0) {
               g_ssThis.SetDebugMode();

               // Execute the service code
               ServiceMain(0, NULL);
            }

            if (lstrcmpi(&ppArgv[i][1], TEXT("service")) == 0) {
               // Connect to the service control dispatcher
               SERVICE_TABLE_ENTRY ServiceTable[] = {
                  { g_szServiceName, ServiceMain },
                  { NULL,            NULL }   // End of list
               };
               chVERIFY(StartServiceCtrlDispatcher(ServiceTable));
            }
         }
      }
   }

#ifdef UNICODE
   HeapFree(GetProcessHeap(), 0, (PVOID) ppArgv);
#endif
   return(0);
}
