// ScheduledTask.h: interface for the CScheduledTask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCHEDULEDTASK_H__6EA4F646_03A4_4E2B_B127_5F7E1E700D30__INCLUDED_)
#define AFX_SCHEDULEDTASK_H__6EA4F646_03A4_4E2B_B127_5F7E1E700D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <time.h>

class CScheduledTask;
template <class CSchedulerLog> class CScheduler;
template <class CSchedulerLog> class CScheduledTaskReConfig;
template <class CSchedulerLog> class CScheduledTaskUrl;

//----------------------------------------------------------------------------------------------------------------

class CScheduledTask  
{
	bool	active;
	
public:
	time_t	laststart;
	_bstr_t m_name,
			m_min,
			m_hour,
			m_day,
			m_month,
			m_wday;

public:
	CScheduledTask() : m_name("unnamed"), m_min("*"), m_hour("*"),
		m_day("*"), m_month("*"), m_wday("*"), active(true)
	{
		laststart = 0L;
	}
	virtual ~CScheduledTask() {;} 

	// копирование
	CScheduledTask(const CScheduledTask& task) {
		*this = task;
	}
	CScheduledTask& operator =( const CScheduledTask& task ) {
		this->m_name = task.m_name;
		this->m_min = task.m_min;
		this->m_hour = task.m_hour;
		this->m_day = task.m_day;
		this->m_month = task.m_month;
		this->m_wday = task.m_wday;
		this->laststart = task.laststart;
		return *this;
	}

	bool match(tm c)
	{
		// если время последнего старта не определено, то позволяем
		// выполнить задачу
		if( laststart == 0 ) return true;

		tm *s = localtime(&laststart);

		return active && 
			   (abs(s->tm_min - c.tm_min) >= 1 || abs(s->tm_hour - c.tm_hour) >= 1 || 
				abs(s->tm_mday - c.tm_mday) >= 1 || abs(s->tm_mon - c.tm_mon) >= 1) &&
			   matchunit(m_min, s->tm_min, c.tm_min) &&
			   matchunit(m_hour, s->tm_hour, c.tm_hour) &&   
			   matchunit(m_day, s->tm_mday, c.tm_mday) &&
			   matchunit(m_wday, s->tm_wday == 0 ? 7 : s->tm_wday, c.tm_wday == 0 ? 7 : c.tm_wday) &&
			   matchunit(m_month, s->tm_mon, c.tm_mon);
	}

	bool matchunit( _bstr_t pattern, int last_unit, int current_unit )
	{
		char *str_pattern = (char *) pattern;

		// изменение времени на единицу
		if( strcmp(str_pattern, "*") == 0 ) return true;

		// вхождение в интервал времени
		char *delimiter = strstr(str_pattern, "-");
		if( delimiter ) 
		{
			char str_begin[10] = {0};
			strncpy(str_begin, str_pattern, __min(sizeof(str_begin) - 1, (delimiter - str_pattern) * sizeof(TCHAR)));

			int begin_period = atoi(str_begin);
			int end_period = atoi(delimiter + 1);

			return current_unit >= begin_period && current_unit <= end_period;
		}

		// повтор через интервал времени
		delimiter = strstr(str_pattern, "/");
		if( delimiter ) 
		{
			int time_step = atoi(delimiter + 1);
			return abs(current_unit - last_unit) >= time_step;
		}

		// наступление момента времени
		int unit = atoi(str_pattern);
		if( unit >= 0 ) return unit == current_unit;

		return false;
	}

	bool execute()
	{
		if( execute_internal() ) {
			time(&laststart);
		}
		else return false;

		return true;
	}

	virtual void setactive(bool act) { active = act; }

protected:
	virtual bool execute_internal() = 0;
};

typedef std::vector<CScheduledTask*> t_tasks;

//----------------------------------------------------------------------------------------------------------------

template <class CSchedulerLog>
class CScheduledTaskReConfig : public CScheduledTask
{
	CScheduler<CSchedulerLog> &m_scheduler;

public:
	CScheduledTaskReConfig( CScheduler<CSchedulerLog> &scheduler ) : m_scheduler(scheduler),
		CScheduledTask() 
	{
		m_name = "scheduler reconfiguration";
		m_min = "*/5";
	}
	virtual ~CScheduledTaskReConfig() {;}
	virtual void setactive(bool act) {;}

protected:
	virtual bool execute_internal() 
	{
		m_scheduler.init();

		return true;
	}
};

//----------------------------------------------------------------------------------------------------------------
template <class CSchedulerLog>
class CScheduledTaskUrl : public CScheduledTask
{
	_bstr_t m_url;
	CSchedulerLog m_log;

public:
	CScheduledTaskUrl( _bstr_t url, CSchedulerLog &log ) : m_url(url), m_log(log), 
		CScheduledTask() {;}
	virtual ~CScheduledTaskUrl() {;}

protected:
	virtual bool execute_internal() 
	{
		try {
			rebus.buildProject(m_url);
		}
		catch(_com_error e)
		{
			m_log.write("TASK EXECUTION ERROR: ", e);
		}
	
		return true;
	}
};


//----------------------------------------------------------------------------------------------------------------
template <class CSchedulerLog>
class CScheduler
{
	friend DWORD WINAPI httprequestthread(LPVOID lpv);

protected:
	MSXML2::IXMLDOMDocument2Ptr m_xml;
	_bstr_t m_config_path;

public:
	CSchedulerLog &m_log;

	CScheduler(CSchedulerLog &log): m_log(log), m_config_path("")
	{
		m_xml.CreateInstance(MSXML2::CLSID_DOMDocument26);

		// в список заданий добавляем задание по запросу
		// списка заданий
		tasks.push_back( new CScheduledTaskReConfig<CSchedulerLog>(*this) );

	}
	virtual ~CScheduler() 
	{
		for(t_tasks::iterator it = tasks.begin(); it != tasks.end(); ++it) {
			delete (*it);
		}
		tasks.clear();
	}

	bool config()
	{
		// load configuration file
		try {
			m_config_path = rebus.getRootPath();

			if(m_config_path.length() < 1) {
				m_log.write("can't get REBUS configuration path.");
				return false;
			}

			m_config_path += "\\config\\config.xml";

			m_xml->load( m_config_path );

			return m_xml->documentElement != NULL;
		}
		catch( _com_error e ) {
			m_log.write("can't load REBUS configuration file", e);
		}
		catch(...) {
			m_log.write("unknown exception");
		}

		return false;
	}

	void save_config()
	{
		try {
			if(m_xml->documentElement != NULL) {
				m_xml->save(m_config_path);
			}
		}
		catch( _com_error e ) {
			m_log.write("can't update REBUS configuration file", e);
		}
		catch(...) {
			m_log.write("unknown exception");
		}
	}

	void init()
	{
		//m_log.write("scheduler init begin");

		// load task list
		if(!config()) {
			m_log.write("REBUS configuration file is not valid.");
			return;
		}

		MSXML2::IXMLDOMNodeListPtr list = m_xml->documentElement->selectNodes("taskinfo/task");
		MSXML2::IXMLDOMElementPtr item;
		
		// деактивируем все задания
		for(t_tasks::iterator it = tasks.begin(); it != tasks.end(); ++it) {
			(*it)->setactive(false);
		}

		// активируем (или создаем) те задания, которые есть в списке заданий
		_bstr_t name;
		CScheduledTask *task;

		//m_log.write("activate tasks");

		for( int i = 0; i < list->length; i++ )
		{
			item = list->item[i];
			name = item->selectSingleNode("name")->text;

			task = NULL;
			for(t_tasks::iterator it = tasks.begin(); it != tasks.end(); ++it) {
				if( (*it)->m_name == name ) {
					(*it)->setactive(true);
					task = (*it);
					break;
				}
			}

			if( !task ) {
				task = new CScheduledTaskUrl<CSchedulerLog>(item->selectSingleNode("id")->text, m_log);
				task->m_name = name;

				// определяем время последнего запуска задания
				_bstr_t strlast = item->selectSingleNode("last")->text;
				tm last;

				if( strlast == _bstr_t("") ) {
					time(&task->laststart);
				}
				else {
					last = *localtime(&task->laststart);

					sscanf((char *) strlast, "%d.%d %d:%d",
						&last.tm_mday, &last.tm_mon, &last.tm_hour, &last.tm_min);
				
					task->laststart = mktime( &last );
				}
				
				tasks.push_back(task);
			}

			task->m_min = canonize(item->selectSingleNode("min")->text);
			task->m_hour = canonize(item->selectSingleNode("hour")->text);
			task->m_day = canonize(item->selectSingleNode("day")->text);
			task->m_month = canonize(item->selectSingleNode("month")->text);
			task->m_wday = canonize(item->selectSingleNode("wday")->text);
		}

		//m_log.write("ends sheduler init");

	}

	_bstr_t& canonize(_bstr_t &val )
	{
		if( val == _bstr_t("") ) val = _bstr_t("*");
		return val;
	}

	void execute( int iTaskId )
	{
		tm ct;
		time_t lt = 0;

		time(&lt);
		ct = *(localtime(&lt));

		if( tasks[iTaskId]->match(ct) ) 
		{
			tasks[iTaskId]->execute();
			
			if(tasks[iTaskId]->m_name != _bstr_t("scheduler reconfiguration")) 
			{
				char lpszMessage[1024] = {0};
				sprintf(lpszMessage, "executed: project %s", (char *) tasks[iTaskId]->m_name);
				m_log.write(lpszMessage);
			}

			// save the last execution time of a task
			MSXML2::IXMLDOMElementPtr task_last_tag = 
				m_xml->documentElement->selectSingleNode("taskinfo/task[name/.='"+ tasks[iTaskId]->m_name +"']/last");
			if(task_last_tag != NULL) 
			{
				char time_str[32] = {0};
				sprintf(time_str, "%d.%d %d:%d", ct.tm_mday, ct.tm_mon, ct.tm_hour, ct.tm_min);
				task_last_tag->text = _bstr_t(time_str);

				save_config();
			}
		}
		/*
		else {
			
			tm *t = localtime(&m_scheduler.tasks[i]->laststart);

			sprintf(lpszMessage, "too early: %s [%s.%s.%s.%s] - lasttime [%d.%d]", (char *) m_scheduler.tasks[i]->m_name, 
				(char *) m_scheduler.tasks[i]->m_min, (char *) m_scheduler.tasks[i]->m_hour, 
				(char *) m_scheduler.tasks[i]->m_day, (char *) m_scheduler.tasks[i]->m_month,
				t->tm_min, t->tm_hour);

			m_scheduler.m_log.write(lpszMessage);
		}
		*/
	}

public:
	t_tasks tasks;
};

#endif // !defined(AFX_SCHEDULEDTASK_H__6EA4F646_03A4_4E2B_B127_5F7E1E700D30__INCLUDED_)
