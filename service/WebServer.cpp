// WebServer.cpp: implementation of the CWebServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebServer.h"

#ifdef _WIN32
#include <winsock.h>
#define	snprintf			_snprintf
#pragma comment(lib,"ws2_32")
#else
#include <sys/types.h>
#include <sys/select.h>
#endif

#include "../common/stringex.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "shttpd.h"
#include "wtpl2.h"

#include "rebus.h"

//////////////////////////////////////////////////////////////////////

#define NVL(x) x ? x : ""

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
LPCTSTR gpszProductSalt1 = "{E7AE23D2-12AE-447a-99A0-0972A8EFA7C9}";

static int 
response_page(struct shttpd_callback_arg *arg, wtpl* tpl, const char *title)
{
	wtpl* tpl_head = new wtpl((char *) rebus.getPage("header"));

	tpl_head->assign("title", title);
	tpl_head->parse("title");

	// output a standard header of page
	int n = snprintf(arg->buf, arg->buflen, tpl_head->text().c_str());
	delete tpl_head;

	// output a page body
	n += snprintf(arg->buf + n, arg->buflen - n, tpl->text().c_str());
	delete tpl;

	if(rebus.m_regInfo->GetEvaluationTimeLeft() > 0) {
		n += snprintf(arg->buf + n, arg->buflen - n, 
			"</td></tr><tr><td class=eval>EVALUATION VERSION: %d more days, please <a class=evallnk href=\"/register\">REGISTER</a>", 
			rebus.m_regInfo->GetEvaluationTimeLeft());
	}

	wtpl* tpl_foot = new wtpl((char *) rebus.getPage("footer"));

	n += snprintf(arg->buf + n, arg->buflen - n, tpl_foot->text().c_str());
	delete tpl_foot;

	return n;
}

static int 
report_error(struct shttpd_callback_arg *arg, _com_error &e)
{
	char buffer[32] = {0};

	wtpl* tpl = new wtpl((char *) rebus.getPage("error"));

	tpl->assign("code", itoa(e.Error(), buffer, 16));

	IErrorInfo *p_errinf = e.ErrorInfo();
	if(p_errinf) 
	{
		if(e.Description().length() > 0)
			tpl->assign("description", (char *) e.Description());
		if(e.Source().length() > 0)
			tpl->assign("source", (char *) e.Source());
		p_errinf->Release();
	}
	else
	{
		tpl->assign("description", (char *) NVL(e.ErrorMessage()));
		tpl->assign("source", "");
	}
	
	tpl->parse("errorinfo");

	// write error info to the log file
	CWebServer<CLogFile> *pServer = (CWebServer<CLogFile> *) arg->callback_data;

	pServer->m_log.write("ERROR: ", e);

	std::string what;
	what = NVL(shttpd_get_agent(arg->connection));
	pServer->m_log.write(what.c_str());

	what = "query string: ";
	what += NVL(shttpd_get_query(arg->connection));
	pServer->m_log.write(what.c_str());
	
	return response_page(arg, tpl, "Error");
}

static int 
redirect_page(struct shttpd_callback_arg *arg, const char *location)
{
	return snprintf(arg->buf, arg->buflen, "HTTP/1.1 %d %s\r\nLocation: %s\r\n\r\n",
		302, "", location);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int
page_default(struct shttpd_callback_arg *arg)
{
	char buffer[32] = {0};
	LPCSTR codename, action;
	bool balive = false;

	try
	{
		if ((codename = shttpd_get_var(arg->connection, "codename")) != NULL &&
			(action = shttpd_get_var(arg->connection, "action")) != NULL)
		{
			rebus.removeProject(codename);
			return redirect_page(arg, "/");
		}

		wtpl* tpl = new wtpl((char *) rebus.getPage("default"));

		int projects_num = rebus.getProjectsCount();
		for( int i = 0; i < projects_num; i++ )
		{
			// get the project reference
			ProjectTypeLib::Automation2Ptr project = rebus.getProject(i);
			bool b_prj_alive = project->isalive(_variant_t());

			tpl->assign("project_id", itoa(i, buffer, 10));

			// get date of last build
			_bstr_t last_date = project->getLastBuildDate(_variant_t());
			if(!b_prj_alive && last_date.length() > 0) 
			{
				tpl->assign("date", (char *) last_date);
				tpl->parse("project.lastbuild");
			}

			tpl->assign("product", (char *) _bstr_t(project->Getproduct()));
			tpl->assign("description", (char *) _bstr_t(project->Getdescription()));
			tpl->assign("codename", (char *) _bstr_t(project->Getcodename()));
			tpl->parse("project");

			if(b_prj_alive) 
			{
				tpl->assign("product", (char *) _bstr_t(project->Getproduct()));
				tpl->assign("project_id", itoa(i, buffer, 10));

				// get build priority
				tpl->assign("priority", (char *) rebus.getBuildPriority(buffer));

				tpl->parse("alive.aliveproject");
				balive = true;
			}
		}
		
		if(balive) {
			tpl->parse("alive");
		}

		return response_page(arg, tpl, "Main");
	}
	catch(_com_error e)
	{
		return report_error( arg, e );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int
page_create(struct shttpd_callback_arg *arg)
{
	char buffer[32] = {0};
	LPCTSTR value = NULL;
	_bstr_t codename(""), path(""), product(""), description(""), stemplate("");
	
	if ((value = shttpd_get_var(arg->connection, "codename")) != NULL)
		codename = value;
	if ((value = shttpd_get_var(arg->connection, "path")) != NULL)
		path = value;
	if ((value = shttpd_get_var(arg->connection, "product")) != NULL)
		product = value;
	if ((value = shttpd_get_var(arg->connection, "description")) != NULL)
		description = value;
	if ((value = shttpd_get_var(arg->connection, "template")) != NULL)
		stemplate = value;

	try 
	{
		if( codename != _bstr_t("") || path != _bstr_t("") )
		{
			// create new project
			long index = rebus.CreateProject( codename, path, product, description, stemplate);

			// redirect to the project configuration page
			return snprintf(arg->buf, arg->buflen, "HTTP/1.1 %d %s\r\nLocation: /project?project_id=%d\r\n\r\n",
				302, "", index);
		}

		wtpl* tpl = new wtpl((char *) rebus.getPage("create"));

		// enum templates
		for(int i = 0; i < rebus.getTemplateCount(); i++)
		{
			tpl->assign("template", (char *) rebus.getTemplate(i));
			tpl->parse("templates");
		}

		// assign default parameters
		MSXML2::IXMLDOMDocumentPtr config = rebus.config();

		tpl->assign("path", (char *) config->selectSingleNode("/rebus/settings/path")->text);
		tpl->parse("default");

		return response_page(arg, tpl, "New Project");
	}
	catch(_com_error e)
	{
		return report_error( arg, e );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int
page_prepeare(struct shttpd_callback_arg *arg)
{
	LPCTSTR value = NULL;

	if ((value = shttpd_get_var(arg->connection, "project_id")) != NULL)
	{
		try
		{
			rebus.buildProject(_bstr_t(value));

			// make pause to prepeare build log 
			Sleep(2000);

			return snprintf(arg->buf, arg->buflen, "HTTP/1.1 %d %s\r\nLocation: /build?project_id=%d\r\n\r\n",
				302, "", atoi(value));
		}
		catch(_com_error e)
		{
			return report_error( arg, e );
		}
	}
	else {
		return redirect_page(arg, "/");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int
page_task(struct shttpd_callback_arg *arg)
{
	try
	{
		MSXML2::IXMLDOMDocumentPtr config = rebus.config();

		LPCTSTR value = NULL;
		LPCTSTR task_id = NULL;

		if ((value = shttpd_get_var(arg->connection, "action")) != NULL)
		{
			if(strcmp(value, "create") == 0)
			{
				MSXML2::IXMLDOMElementPtr id = config->createElement("id");
				id->text = _bstr_t(shttpd_get_var(arg->connection, "id"));

				MSXML2::IXMLDOMElementPtr name = config->createElement("name");
				name->text = _bstr_t(rebus.getProject(atol(shttpd_get_var(arg->connection, "id")))->Getproduct());

				MSXML2::IXMLDOMElementPtr min = config->createElement("min");
				min->text = _bstr_t(shttpd_get_var(arg->connection, "Minute"));

				MSXML2::IXMLDOMElementPtr hour = config->createElement("hour");
				hour->text = _bstr_t(shttpd_get_var(arg->connection, "Hour"));

				MSXML2::IXMLDOMElementPtr day = config->createElement("day");
				day->text = _bstr_t(shttpd_get_var(arg->connection, "Day"));

				MSXML2::IXMLDOMElementPtr month = config->createElement("month");
				month->text = _bstr_t(shttpd_get_var(arg->connection, "Month"));

				MSXML2::IXMLDOMElementPtr wday = config->createElement("wday");
				wday->text = _bstr_t(shttpd_get_var(arg->connection, "Wday"));

				MSXML2::IXMLDOMElementPtr last = config->createElement("last");

				MSXML2::IXMLDOMElementPtr task = config->createElement("task");
				task->appendChild(id);
				task->appendChild(name);
				task->appendChild(min);
				task->appendChild(hour);
				task->appendChild(day);
				task->appendChild(month);
				task->appendChild(wday);
				task->appendChild(last);

				config->selectSingleNode("/rebus/taskinfo")->appendChild(task);
			}
			else if(strcmp(value, "delete") == 0 &&
				   (task_id = shttpd_get_var(arg->connection, "task_id")) != NULL)
			{
				MSXML2::IXMLDOMElementPtr taskinfo = config->selectSingleNode("/rebus/taskinfo");
				taskinfo->removeChild(taskinfo->selectSingleNode("task["+ _bstr_t(task_id) +"]"));
			}

			rebus.saveConfig();

			return redirect_page(arg, "task");
		}
		else 
		{
			char buffer[32] = {0};
			wtpl* tpl = new wtpl((char *) rebus.getPage("task"));
	
			// enum tasks
			MSXML2::IXMLDOMNodeListPtr list = config->selectNodes("/rebus/taskinfo/task");
			for(int j = 0; j < list->length; j++)
			{
				MSXML2::IXMLDOMElementPtr task = list->item[j];

				ProjectTypeLib::Automation2Ptr project = 
					rebus.getProject(atol((char *) task->selectSingleNode("id")->text));

				tpl->assign("product", (char *) (char *) _bstr_t(project->Getproduct()) );
				tpl->assign("min", (char *) task->selectSingleNode("min")->text );
				tpl->assign("hour", (char *) task->selectSingleNode("hour")->text );
				tpl->assign("day", (char *) task->selectSingleNode("day")->text );
				tpl->assign("month", (char *) task->selectSingleNode("month")->text );
				tpl->assign("wday", (char *) task->selectSingleNode("wday")->text );
				tpl->assign("task_id", itoa(j, buffer, 10) );
				tpl->parse("task");
			}

			// enum projects for task form
			int projects_num = rebus.getProjectsCount();
			for( int i = 0; i < projects_num; i++ )
			{
				ProjectTypeLib::Automation2Ptr project = rebus.getProject(i);

				tpl->assign("project_id", itoa(i, buffer, 10));
				tpl->assign("product", (char *) _bstr_t(project->Getproduct()));
				tpl->parse("project");
			}

			return response_page(arg, tpl, "Scheduled Tasks");
		}
	}
	catch(_com_error e)
	{
		return report_error( arg, e );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int
page_build(struct shttpd_callback_arg *arg)
{
	LPCTSTR value = NULL;

	if ((value = shttpd_get_var(arg->connection, "project_id")) != NULL)
	{
		try
		{
			_bstr_t error("");
			wtpl* tpl = new wtpl((char *) rebus.getPage("build"));
		
			// get the project reference
			ProjectTypeLib::Automation2Ptr project = 
				rebus.getProject(atoi(value));

			if((bool) project->isalive(_variant_t())) {
				tpl->parse("refresh");

				tpl->assign("project_id", value);
				tpl->parse("stop");
			}
			else {
				error = project->errorText(_variant_t());

				// parse actions
				tpl->assign("project_id", value);
				tpl->parse("actions");
			}

			// set output info
			tpl->assign("log", (char *) _bstr_t(project->output(_variant_t())));
			tpl->assign("error", (char *) error);
			tpl->parse("output");

			return response_page(arg, tpl, "Build State");
		}
		catch(_com_error e)
		{
			return report_error( arg, e );
		}
	}
	else {
		return redirect_page(arg, "/");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int
page_project(struct shttpd_callback_arg *arg)
{
	try
	{
		LPCTSTR project_id = NULL;
		LPCTSTR action = NULL;

		if ((project_id = shttpd_get_var(arg->connection, "project_id")) == NULL)
			return redirect_page(arg, "/");

		ProjectTypeLib::Automation2Ptr project = 
			rebus.getProject(atol(project_id));
		
		if ((action = shttpd_get_var(arg->connection, "action")) != NULL)
		{
			if(stricmp(action, "newplugin") == 0)
			{
				project->appendPlugin( _bstr_t(shttpd_get_var(arg->connection, "PluginClass")) );
			}
			else if(stricmp(action, "delete") == 0)
			{
				project->removePlugin( _bstr_t(shttpd_get_var(arg->connection, "plugin")) );
			}
			else if(stricmp(action, "setparms") == 0)
			{
				char argname[255] = {0},
					 position[32] = {0},
					 argvalue[255] = {0};

				for(int i = 0; i < atoi(shttpd_get_var(arg->connection, "plugin_num")); i++)
				{
					sprintf(argname, "plugin_%d_parms", i);
					int parms = atoi(shttpd_get_var(arg->connection, argname));

					for(int j = 0; j < parms; j++)
					{
						sprintf(argname, "plugin_%d_parm_%d", i, j);
						sprintf(argvalue, "plugin_%d_%s", i, shttpd_get_var(arg->connection, argname));
						
						project->setPluginParm( _bstr_t(itoa(i, position, 10)),
							_bstr_t(shttpd_get_var(arg->connection, argname)),
							_bstr_t(shttpd_get_var(arg->connection, argvalue)),
							_variant_t() );

					}

					sprintf(argname, "enabled_%d", i);
					project->enablePlugin( _bstr_t(itoa(i, position, 10)),
						_variant_t(stricmp(NVL(shttpd_get_var(arg->connection, argname)), "on") == 0),
						_variant_t());

					// assign project attributes
					MSXML2::IXMLDOMDocumentPtr config = 
						rebus.getProjectConfig(atol(project_id));

					if(strcmp(shttpd_get_var(arg->connection, "product"), "") != 0) {
						config->selectSingleNode("/project/productname")->text = 
							_bstr_t(shttpd_get_var(arg->connection, "product"));
					}

					config->selectSingleNode("/project/description")->text = 
						_bstr_t(NVL(shttpd_get_var(arg->connection, "description"), ""));

					config->selectSingleNode("/project/email")->text = 
						_bstr_t(NVL(shttpd_get_var(arg->connection, "email"), ""));

					if(strcmp(shttpd_get_var(arg->connection, "version"), "") != 0) {
						config->selectSingleNode("/project/version")->text = 
							_bstr_t(shttpd_get_var(arg->connection, "version"));
					}
				}

				project->save(_variant_t());

				if(stricmp(shttpd_get_var(arg->connection, "persistent"), "1") != 0) 
				{
					return snprintf(arg->buf, arg->buflen, "HTTP/1.1 %d %s\r\nLocation: /prepeare?project_id=%s\r\n\r\n",
						302, "", project_id);
				}
			}
			else if(stricmp(action, "moveup") == 0)
			{
				project->moveUpPlugin( _bstr_t(shttpd_get_var(arg->connection, "plugin")) );
			}
			else if(stricmp(action, "movedown") == 0)
			{
				project->moveDownPlugin( _bstr_t(shttpd_get_var(arg->connection, "plugin")) );
			}

			return snprintf(arg->buf, arg->buflen, "HTTP/1.1 %d %s\r\nLocation: /project?project_id=%s\r\n\r\n",
				302, "", project_id);
		}
		else 
		{
			char buffer[32] = {0};
			wtpl* tpl = new wtpl((char *) rebus.getPage("project"));
	
			// enum project's plugins
			MSXML2::IXMLDOMDocumentPtr config = 
				rebus.getProjectConfig(atol(project_id));

			MSXML2::IXMLDOMNodeListPtr plugins = 
				config->selectNodes("/project/plugin");

			for(int j = 0; j < plugins->length; j++)
			{
				MSXML2::IXMLDOMElementPtr plugin = plugins->item[j];

				tpl->assign("index", string(itoa(j, buffer, 10)).c_str());
				try {
					if(_bstr_t(plugin->getAttribute("active")) == _bstr_t("yes"))
						tpl->assign("checked", "checked");
					else
						tpl->assign("checked", "");
				}
				catch(...) {
					tpl->assign("checked", "");
				}
				tpl->assign("class", (char *) plugins->item[j]->selectSingleNode("class")->text);
	
				if(j > 0) {
					tpl->assign("project_id", project_id);
					tpl->parse("project.plugin.moveup");
				}
				if(j < plugins->length - 1) {
					tpl->assign("project_id", project_id);
					tpl->parse("project.plugin.movedown");
				}

				// parse plugin parameters
				MSXML2::IXMLDOMDocumentPtr def = 
					rebus.getPluginParmsDef( plugins->item[j]->selectSingleNode("class")->text );

				int parms_count = 0;
				if(def->documentElement != NULL) 
				{
					MSXML2::IXMLDOMNodeListPtr parms = def->documentElement->childNodes;
					parms_count = parms->length;

					for(int k = 0; k < parms->length; k++)
					{
						tpl->assign("name", (char *) _bstr_t(parms->item[k]->nodeName));
						tpl->assign("parm_index", string(itoa(k, buffer, 10)).c_str());
						tpl->assign("index", string(itoa(j, buffer, 10)).c_str());

						// get plugin value
						_bstr_t value = project->getPluginParm( _bstr_t(itoa(j, buffer, 10)),
							parms->item[k]->nodeName, _variant_t() );

						// check for parameter type
						string parmdef = (char *) parms->item[k]->text;
						vector<string> defitems;
						stringex::split(parmdef, defitems, ";");

						if(defitems.size() > 0 && strstr(defitems[0].c_str(), ":")) 
						{
							// selectable value
							for(vector<string>::iterator it = defitems.begin(); it != defitems.end(); ++it)
							{
								vector<string> option;
								stringex::split((*it), option, ":");

								if(option.size() > 1) {
									tpl->assign("value", option[0].c_str());
									tpl->assign("name", option[1].c_str());

									if(stricmp(option[0].c_str(), value) == 0) {
										tpl->assign("selected", "selected");
									} else {
										tpl->assign("selected", "");
									}
									tpl->parse("project.plugin.parameter.selector.option");
								}
							}

							tpl->assign("name", (char *) _bstr_t(parms->item[k]->nodeName));
							tpl->assign("value", (char *) value);
							tpl->assign("plugin_index", string(itoa(j, buffer, 10)).c_str());

							tpl->parse("project.plugin.parameter.selector");
						}
						else 
						{
							// simple value
							if(value.length() < 1) {
								value = parms->item[k]->text;
							}
							tpl->assign("name", (char *) _bstr_t(parms->item[k]->nodeName));
							tpl->assign("value", (char *) value);
							tpl->assign("plugin_index", string(itoa(j, buffer, 10)).c_str());

							tpl->parse("project.plugin.parameter.simple");
						}

						tpl->parse("project.plugin.parameter");
					}
				}

				tpl->assign("project_id", project_id);
				tpl->assign("parameters", string(itoa(parms_count, buffer, 10)).c_str());
				tpl->parse("project.plugin");

			}

			// enum plugins
			int plugins_num = rebus.getPluginCount();
			for( int i = 0; i < plugins_num; i++ )
			{
				_bstr_t plugin = rebus.getPlugin(i);

				tpl->assign("plugin", (char *) plugin);
				tpl->assign("project_id", project_id);
				tpl->parse("project.allplugins");
			}

			tpl->assign("plugin_num", itoa(plugins->length, buffer, 10));
			tpl->assign("project_id", project_id);
			tpl->assign("product", (char *) config->selectSingleNode("/project/productname")->text);
			tpl->assign("path", (char *) _bstr_t(project->Getpath()));
			tpl->assign("codename", (char *) _bstr_t(project->Getcodename()));
			tpl->assign("description", (char *) config->selectSingleNode("/project/description")->text);
			tpl->assign("version", (char *) config->selectSingleNode("/project/version")->text);
			tpl->assign("email", (char *) config->selectSingleNode("/project/email")->text);

			tpl->parse("project");

			return response_page(arg, tpl, "Project Configuration");
		}
	}
	catch(_com_error e)
	{
		return report_error( arg, e );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int
page_stop(struct shttpd_callback_arg *arg)
{
	LPCTSTR value = NULL;

	if ((value = shttpd_get_var(arg->connection, "project_id")) != NULL)
	{
		try
		{
			rebus.getProject(atoi(value))->stop(_variant_t());

			// make pause to prepeare build log 
			Sleep(2000);

			return snprintf(arg->buf, arg->buflen, "HTTP/1.1 %d %s\r\nLocation: /build?project_id=%d\r\n\r\n",
				302, "", atoi(value));
		}
		catch(_com_error e)
		{
			return report_error( arg, e );
		}
	}
	else {
		return redirect_page(arg, "/");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int
page_settings(struct shttpd_callback_arg *arg)
{
	try
	{
		MSXML2::IXMLDOMDocumentPtr config = rebus.config();
		LPCTSTR value = NULL;

		if ((value = shttpd_get_var(arg->connection, "action")) != NULL)
		{
			if(stricmp(value, "save") == 0)
			{
				MSXML2::IXMLDOMElementPtr node = 
					config->selectSingleNode("/rebus/settings/port");
				node->text = _bstr_t(NVL(shttpd_get_var(arg->connection, "port")));

				node = config->selectSingleNode("/rebus/settings/company");
				node->text = _bstr_t(NVL(shttpd_get_var(arg->connection, "company")));

				node = config->selectSingleNode("/rebus/settings/copyright");
				node->text = _bstr_t(NVL(shttpd_get_var(arg->connection, "copyright")));

				node = config->selectSingleNode("/rebus/settings/email");
				node->text = _bstr_t(NVL(shttpd_get_var(arg->connection, "email")));

				std::string path(NVL(shttpd_get_var(arg->connection, "path")));
				if((*path.rbegin()) != '\\' && (*path.rbegin()) != '/') {
					path += "\\";
				}
				node = config->selectSingleNode("/rebus/settings/path");
				node->text = _bstr_t(path.c_str());

				rebus.saveConfig();
			}
			return redirect_page(arg, "settings");
		}
		else 
		{
			wtpl* tpl = new wtpl((char *) rebus.getPage("settings"));
	
			tpl->assign("port", (char *) config->selectSingleNode("/rebus/settings/port")->text);
			tpl->assign("company", (char *) config->selectSingleNode("/rebus/settings/company")->text);
			tpl->assign("copyright", (char *) config->selectSingleNode("/rebus/settings/copyright")->text);
			tpl->assign("path", (char *) config->selectSingleNode("/rebus/settings/path")->text);
			tpl->assign("email", (char *) config->selectSingleNode("/rebus/settings/email")->text);
			tpl->parse("settings");

			return response_page(arg, tpl, "Settings");
		}
	}
	catch(_com_error e)
	{
		return report_error( arg, e );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int
page_register(struct shttpd_callback_arg *arg)
{
	try
	{
		LPCTSTR company = shttpd_get_var(arg->connection, "company"), 
				order = shttpd_get_var(arg->connection, "order"), 
				key = shttpd_get_var(arg->connection, "key");
		_bstr_t info(""), color("");

		if (company && order && key)
		{
			registry.SetMachineEntry( "REBUS", "UserData", CString(order) );
			registry.SetMachineEntry( "REBUS", "ProductKey", CString(key) );
			registry.SetMachineEntry( "REBUS", "CompanyName", CString(company) );

			// reset the registration info
			rebus.m_regInfo->ResetRegistrationInfo( order, key );

			// check the registration state
			if(rebus.m_regInfo->IsRegistered() && rebus.m_regInfo->GetEvaluationTimeLeft() < 1)
			{
				info = "The registration of the product has been completed successfully. Please restart the REBUS service.";
				color = "green";
			}
			else {
				info = "The registration of the product has been failed. Please, check your order number or a product key.";
				color = "red";
			}
		}

		wtpl* tpl = new wtpl((char *) rebus.getPage("register"));
	
		if(info.length() > 0) {
			tpl->assign("info", (char *) info);
			tpl->assign("color", (char *) color);
			tpl->parse("registration.complete");
		}

		tpl->assign("company", NVL(company));
		tpl->assign("order", NVL(order));
		tpl->assign("key", NVL(key));
		tpl->parse("registration");

		return response_page(arg, tpl, "Product Registration");
	}
	catch(_com_error e)
	{
		return report_error( arg, e );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int
page_help(struct shttpd_callback_arg *arg)
{
	try
	{
		wtpl* tpl = new wtpl((char *) rebus.getPage("help"));
	
		return response_page(arg, tpl, "Help");
	}
	catch(_com_error e)
	{
		return report_error( arg, e );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int
page_priority(struct shttpd_callback_arg *arg)
{
	try
	{
		LPCTSTR project_id = NULL, direction = NULL;

		if ((project_id = shttpd_get_var(arg->connection, "project_id")) != NULL) {
			if ((direction = shttpd_get_var(arg->connection, "dir")) != NULL) 
			{
				// change build process priority
				rebus.changeBuildPriority(_bstr_t(project_id), (int) (strcmp(direction, "plus") == 0 ? 1 : -1));
			}
		}
	
		return redirect_page(arg, "/");
	}
	catch(_com_error e)
	{
		return report_error( arg, e );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <>
CWebServer<CLogFile>::CWebServer(Log &log) : m_log(log)
{
	init();
}

template <>
CWebServer<CLogFile>::~CWebServer(void)
{
}

template <>
void CWebServer<CLogFile>::loop()
{
	shttpd_poll(m_sock, 200);
}

template <>
void CWebServer<CLogFile>::init()
{
	// initialize web server
	shttpd_init(NULL);

	// setup web server configuration
	shttpd_setopt("document_root", ".");

	shttpd_register_mountpoint("/_img", rebus.getRootPath() + "\\pages\\_img");
	shttpd_register_mountpoint("/docs", rebus.getRootPath() + "\\docs");

	if(rebus.m_regInfo->IsRegistered())
	{
		// register web pages
		shttpd_register_url("/", &page_default, (LPVOID) this);
		shttpd_register_url("/create", &page_create, (LPVOID) this);
		shttpd_register_url("/prepeare", &page_prepeare, (LPVOID) this);
		shttpd_register_url("/build", &page_build, (LPVOID) this);
		shttpd_register_url("/stop", &page_stop, (LPVOID) this);
		shttpd_register_url("/task", &page_task, (LPVOID) this);
		shttpd_register_url("/project", &page_project, (LPVOID) this);
		shttpd_register_url("/settings", &page_settings, (LPVOID) this);
		shttpd_register_url("/register", &page_register, (LPVOID) this);
		shttpd_register_url("/help", &page_help, (LPVOID) this);
		shttpd_register_url("/priority", &page_priority, (LPVOID) this);
	}
	else
	{
		shttpd_register_url("/", &page_register, NULL);
	}

	// get the port number
	try {
		MSXML2::IXMLDOMDocumentPtr config = rebus.config();
		MSXML2::IXMLDOMElementPtr port = config->selectSingleNode("/rebus/settings/port");

		// start listening for users access
		if(port != NULL) {
			m_sock = shttpd_open_port(atoi((char *) port->text));
		}
		else {
			m_sock = shttpd_open_port(9000);
		}

		m_log.write("http server is started");
	}
	catch(_com_error e)
	{
		m_log.write("Can't get REBUS config file", e);
	}
}

template <>
void CWebServer<CLogFile>::stop()
{
	shttpd_fini();
	m_log.write("http server is stoped");
}
