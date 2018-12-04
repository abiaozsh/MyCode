package sample;

import java.io.PrintWriter;
import java.nio.charset.Charset;
import java.nio.charset.UnsupportedCharsetException;

import webServer.ConfigServlet;
import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.ServerConfig;
import webServer.Servlet;
import webServer.ServletPack;
import webServer.Session;
import webServer.Util;
import webServer.ServerConfig.FileSystem;
import webServer.ServerConfig.JspProcessor;

if (currentConfig.getConfigPageEnabled() && currentConfig.getConfigPageUrl() != null)
{
	ServletPack sp = new ServletPack();
	sp.clazz = ConfigServlet.class;
	sp.url = currentConfig.getConfigPageUrl();
	servlets.put(currentConfig.getConfigPageUrl(), sp);
}
<configPage>
<enabled>true</enabled>
<url>/config</url>
<userName>admin</userName>
<passWord>uYCu8m0wTlCO+9yXSWeIoA==</passWord>
</configPage>

final class ConfigServlet extends Servlet
{
	@Override
	public void doReq(Request req, Response res, Server server) throws Exception
	{
		Session session = req.getSession();

		String action = req.getPOSTParam("action");

		if ("login".equals(action))
		{
			boolean pass = login(req.getPOSTParam("username"), req.getPOSTParam("password"), server);

			if (pass)
			{
				session.setPrivate("ConfigLogin", "T");
			}
			else
			{
				return;
			}
		}
		else if ("logout".equals(action))
		{
			session.setPrivate("ConfigLogin", null);
			makeLogin(req, res, server);
			return;
		}
		else
		{
			if (!"T".equals(session.getPrivate("ConfigLogin")))
			{
				makeLogin(req, res, server);
				return;
			}
		}

		if ("status".equals(req.getGETParam("action")))
		{
			makeStatus(req, res, server);
			return;
		}

		String scroll = req.getPOSTParam("scroll");
		if (scroll != null && !"".equals(scroll))
		{
			session.setPrivate("config_scroll_position", new Integer(scroll));
		}

		// ----------------------------------------------

		if ("makeXML".equals(action))
		{
			server.currentConfig.SaveConfig(server.configFile);

			makeMsg(req, res, server, "makeXML done");
			return;
		}

		if ("reset".equals(action))
		{
			server.restart();
			makeMsg(req, res, server, "server restarted");
			return;
		}

		if ("shutdown".equals(action))
		{
			server.shutdown();
			makeMsg(req, res, server, "server shutdown");
			return;
		}

		String value = req.getPOSTParam("value");

		if ("set_port".equals(action))
		{
			try
			{
				server.currentConfig.setPort(Integer.parseInt(value));
			}
			catch (Exception e)
			{
				makeMsg(req, res, server, "input number only");
				return;
			}
		}

		if ("set_SSLactive".equals(action))
		{
			try
			{
				server.currentConfig.setSSLactive(Boolean.parseBoolean(value));
			}
			catch (Exception e)
			{
				//
			}
		}

		if ("set_SSLport".equals(action))
		{
			try
			{
				server.currentConfig.setSSLport(Integer.parseInt(value));
			}
			catch (Exception e)
			{
				makeMsg(req, res, server, "input number only");
				return;
			}
		}

		if ("set_SSLclientKeysFile".equals(action))
		{
			server.currentConfig.setSSLclientKeysFile(value);
		}

		if ("set_SSLkeyStorePass".equals(action))
		{
			server.currentConfig.setSSLkeyStorePass(value);
		}

		if ("set_SSLkeyPassword".equals(action))
		{
			server.currentConfig.setSSLkeyPassword(value);
		}

		if ("set_javaCompiler".equals(action))
		{
			if ("".equals(value))
			{
				value = System.getProperty("java.home");
				value = value.substring(0, value.length() - 3);
				value = "file:///" + value + "lib" + Server.SEP + "tools.jar";
			}
			server.currentConfig.setJavaCompiler(value);
		}

		if ("set_defaultEncoding".equals(action))
		{
			try
			{
				Charset.forName(value);
				server.currentConfig.setDefaultEncoding(value);
			}
			catch (UnsupportedCharsetException e)
			{
				makeMsg(req, res, server, "encoding does not supported");
				return;
			}
		}

		if ("set_sessionExpires".equals(action))
		{
			try
			{
				server.currentConfig.setSessionExpires(Integer.parseInt(value));
			}
			catch (Exception e)
			{
				makeMsg(req, res, server, "input number only");
				return;
			}
		}

		if ("set_configPageEnabled".equals(action))
		{
			try
			{
				server.currentConfig.setConfigPageEnabled(Boolean.parseBoolean(value));
			}
			catch (Exception e)
			{
				//
			}
		}

		if ("set_configPage".equals(action))
		{
			synchronized (server.servlets)
			{
				ServletPack sp = server.servlets.remove(server.currentConfig.getConfigPageUrl());
				sp.url = value;
				server.servlets.put(value, sp);
			}
			server.currentConfig.setConfigPageUrl(value);

		}

		if ("set_configPass".equals(action))
		{
			String pwd;
			if (value == null || "".equals(value))
			{
				pwd = "";
			}
			else
			{
				pwd = Util.MD5(value);
			}

			if (pwd.equals(server.currentConfig.getConfigPassWord()))
			{
				String value1 = req.getPOSTParam("value1");

				String value2 = req.getPOSTParam("value2");

				String value3 = req.getPOSTParam("value3");

				if (value1.equals(value2))
				{
					server.currentConfig.setConfigUserName(value3);
					server.currentConfig.setConfigPassWord(Util.MD5(value1));
				}
				else
				{
					makeMsg(req, res, server, "password not equals password confirm");
					return;
				}
			}
			else
			{
				makeMsg(req, res, server, "wrong pass word");
				return;
			}
		}

		if ("del_fileSystem".equals(action))
		{
			int n = Integer.parseInt(value);
			session.removePrivate("edit_fileSystem");
			server.currentConfig.deleteFileSystems(n);
		}

		if ("edit_fileSystem".equals(action))
		{
			int n = Integer.parseInt(value);
			session.setPrivate("edit_fileSystem", new Integer(n));
		}

		if ("upd_fileSystem".equals(action) || "add_fileSystem".equals(action))
		{
			String url = value;
			String rootPath = req.getPOSTParam("value1");
			String allowList = req.getPOSTParam("value2");
			String allowUpLoad = req.getPOSTParam("value3");
			String allowDel = req.getPOSTParam("value4");
			String isDownload = req.getPOSTParam("value5");
			String passWord = req.getPOSTParam("value6");
			String userName = req.getPOSTParam("value7");

			Integer n = (Integer) session.getPrivate("edit_fileSystem");
			if ("".equals(passWord))
			{
				passWord = null;
			}
			if (passWord != null)
			{
				passWord = Util.MD5(passWord);
			}
			if (n != null)
			{
				int l = n.intValue();
				ServerConfig.FileSystem cfs = new ServerConfig.FileSystem();
				cfs.url = url;
				cfs.rootPath = rootPath;
				cfs.allowList = Boolean.parseBoolean(allowList);
				cfs.allowUpLoad = Boolean.parseBoolean(allowUpLoad);
				cfs.allowDel = Boolean.parseBoolean(allowDel);
				cfs.isDownload = Boolean.parseBoolean(isDownload);
				cfs.userName = userName;
				cfs.passWord = passWord;
				server.currentConfig.updateFileSystems(l, cfs);
				session.removePrivate("edit_fileSystem");
			}
			else
			{
				ServerConfig.FileSystem cfs = new ServerConfig.FileSystem();
				cfs.url = url;
				cfs.rootPath = rootPath;
				cfs.allowList = Boolean.parseBoolean(allowList);
				cfs.allowUpLoad = Boolean.parseBoolean(allowUpLoad);
				cfs.allowDel = Boolean.parseBoolean(allowDel);
				cfs.userName = userName;
				cfs.passWord = passWord;
				server.currentConfig.insertFileSystems(cfs);
			}
		}

		if ("del_jspProcessors".equals(action))
		{
			int n = Integer.parseInt(value);
			session.removePrivate("edit_jspProcessors");
			server.currentConfig.deleteJspProcessors(n);
		}

		if ("edit_jspProcessors".equals(action))
		{
			int n = Integer.parseInt(value);
			session.setPrivate("edit_jspProcessors", new Integer(n));
		}

		if ("upd_jspProcessors".equals(action) || "add_jspProcessors".equals(action))
		{
			String url = value;
			String rootPath = req.getPOSTParam("value1");

			Integer n = (Integer) session.getPrivate("edit_jspProcessors");
			if (n != null)
			{
				int l = n.intValue();
				ServerConfig.JspProcessor cjp = new ServerConfig.JspProcessor();
				cjp.url = url;
				cjp.rootPath = rootPath;
				server.currentConfig.updateJspProcessors(l, cjp);
				session.removePrivate("edit_jspProcessors");
			}
			else
			{
				ServerConfig.JspProcessor cjp = new ServerConfig.JspProcessor();
				cjp.url = url;
				cjp.rootPath = rootPath;
				server.currentConfig.insertJspProcessors(cjp);
			}
		}

		if ("del_servlets".equals(action))
		{
			int n = Integer.parseInt(value);
			session.removePrivate("edit_servlets");
			server.currentConfig.deleteServlets(n);
		}

		if ("edit_servlets".equals(action))
		{
			int n = Integer.parseInt(value);
			session.setPrivate("edit_servlets", new Integer(n));
		}

		if ("upd_servlets".equals(action) || "add_servlets".equals(action))
		{
			String url = value;
			String classFileName = req.getPOSTParam("value1");
			boolean isSingleton = Boolean.parseBoolean(req.getPOSTParam("value2"));

			Integer n = (Integer) session.getPrivate("edit_servlets");
			if (n != null)
			{
				int l = n.intValue();
				ServerConfig.Servlet cs = new ServerConfig.Servlet();
				cs.url = url;
				cs.classFileName = classFileName;
				cs.isSingleton = isSingleton;
				server.currentConfig.updateServlets(l, cs);
				session.removePrivate("edit_servlets");
			}
			else
			{
				ServerConfig.Servlet cs = new ServerConfig.Servlet();
				cs.url = url;
				cs.classFileName = classFileName;
				cs.isSingleton = isSingleton;
				server.currentConfig.insertServlets(cs);
			}
		}

		makeConfig(req, res, server);
	}

	private boolean login(String username, String password, Server server)
	{
		String u1, u2, p1, p2;

		if (username == null)
		{
			u1 = "";
		}
		else
		{
			u1 = username;
		}
		if (server.currentConfig.getConfigUserName() == null)
		{
			u2 = "";
		}
		else
		{
			u2 = server.currentConfig.getConfigUserName();
		}

		if (password == null || "".equals(password))
		{
			p1 = "";
		}
		else
		{
			p1 = Util.MD5(password);
		}

		if (server.currentConfig.getConfigPassWord() == null)
		{
			p2 = "";
		}
		else
		{
			p2 = server.currentConfig.getConfigPassWord();
		}

		if (u1.equals(u2) && p2.equals(p1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	private static String getPullDown(String name, boolean value)
	{
		StringBuffer sb = new StringBuffer();
		sb.append("<select id='");
		sb.append(name);
		sb.append("'>");

		sb.append("<option ");
		if (value)
		{
			sb.append("selected ");
		}
		sb.append("value='true' >true</option>");

		sb.append("<option ");
		if (!value)
		{
			sb.append("selected ");
		}
		sb.append("value='false' >false</option>");

		sb.append("</select>");

		return sb.toString();
	}

	private static void makeConfig(Request req, Response res, Server server) throws Exception
	{
		PrintWriter out = res.getWriter();

		Session session = req.getSession();

		out.println("<html>");
		out.println("<head>");
		out.println("<script>");
		out.println("function on_load()");
		out.println("{");
		out.println(" setScroll(document.formMain.scroll.value);");
		out.println("}");
		out.println("function do_action(action,value,value1,value2,value3,value4,value5,value6,value7)");
		out.println("{");
		out.println("	document.formMain.scroll.value = getScroll();");
		out.println("	document.formMain.action.value = action;");
		out.println("	document.formMain.value.value  = value;");
		out.println("	document.formMain.value1.value = value1;");
		out.println("	document.formMain.value2.value = value2;");
		out.println("	document.formMain.value3.value = value3;");
		out.println("	document.formMain.value4.value = value4;");
		out.println("	document.formMain.value5.value = value5;");
		out.println("	document.formMain.value6.value = value6;");
		out.println("	document.formMain.value7.value = value7;");
		out.println("	document.formMain.submit();");
		out.println("}");
		out.println("function getScroll(){");
		out.println(" var t;");
		out.println(" if (document.documentElement && document.documentElement.scrollTop) {");
		out.println("  t = document.documentElement.scrollTop;");
		out.println(" } else if (document.body) {");
		out.println("  t = document.body.scrollTop;");
		out.println(" }");
		out.println(" return t;");
		out.println("}");
		out.println("function setScroll(t){");
		out.println(" if (document.documentElement && document.documentElement.scrollTop) {");
		out.println("  document.documentElement.scrollTop = t;");
		out.println(" } else if (document.body) {");
		out.println("  document.body.scrollTop = t;");
		out.println(" }");
		out.println("}");
		out.println("</script>");
		out.println("</head>");
		out.println("<body onload = \"on_load();\">");
		out.println("<form name = 'formMain' action = '" + server.currentConfig.getConfigPageUrl() + "' method = 'post'>");
		out.println("<input type = hidden name = \"action\" id = \"action\">");
		out.println("<input type = hidden name = \"value\"  id = \"value\">");
		out.println("<input type = hidden name = \"value1\" id = \"value1\">");
		out.println("<input type = hidden name = \"value2\" id = \"value2\">");
		out.println("<input type = hidden name = \"value3\" id = \"value3\">");
		out.println("<input type = hidden name = \"value4\" id = \"value4\">");
		out.println("<input type = hidden name = \"value5\" id = \"value5\">");
		out.println("<input type = hidden name = \"value6\" id = \"value6\">");
		out.println("<input type = hidden name = \"value7\" id = \"value7\">");

		Integer sc = (Integer) session.getPrivate("config_scroll_position");
		int scroll = 0;
		if (sc != null)
		{
			scroll = sc.intValue();
		}
		out.println("<input type = hidden name = \"scroll\" id = \"scroll\" value = \"" + scroll + "\">");
		out.println("</form>");
		out.println("<table border=\"1\">");
		out.println(" <tr>");
		out.println("  <td><a href = \"javascript:do_action('reset');\">Apply config and reset server.</a></td>");
		out.println("  <td><a href = \"javascript:do_action('makeXML');\">Write config to XML.</a></td>");
		out.println("  <td><a href = \"javascript:do_action('shutdown');\">Shutdown server.</a></td>");
		out.println("  <td><a href = \"javascript:do_action('logout');\">Logout.</a></td>");
		out.println("  <td><a href = \"" + server.currentConfig.getConfigPageUrl() + "\" onclick = \"window.open('" + server.currentConfig.getConfigPageUrl() + "?action=status');\">show server status.</a></td>");
		out.println(" </tr>");
		out.println("</table>");
		out.println("<table border=\"1\">");
		out.println(" <tr>");
		out.println("  <td>port:</td>");
		out.println("  <td><input type = text id = 'port' value = '" + server.currentConfig.getPort() + "'></td>");
		out.println("  <td><input type = button value = 'set' onclick = \"do_action('set_port',port.value);\"></td>");
		out.println(" </tr>");
		out.println(" <tr>");
		out.println("  <td>SSLactive:</td>");
		out.println("  <td>" + getPullDown("SSLactive", server.currentConfig.getSSLactive()) + "</td>");
		out.println("  <td><input type = button value = 'set' onclick = \"do_action('set_SSLactive',SSLactive.value);\"></td>");
		out.println(" </tr>");
		if (server.currentConfig.getSSLactive())
		{
			out.println(" <tr>");
			out.println("  <td>SSLport:</td>");
			out.println("  <td><input type = text id = 'SSLport' value = '" + server.currentConfig.getSSLport() + "'></td>");
			out.println("  <td><input type = button value = 'set' onclick = \"do_action('set_SSLport',SSLport.value);\"></td>");
			out.println(" </tr>");
			out.println(" <tr>");
			out.println("  <td>clientKeysFile:</td>");
			out.println("  <td><input type = text id = 'SSLclientKeysFile' value = '" + server.currentConfig.getSSLclientKeysFile() + "'></td>");
			out.println("  <td><input type = button value = 'set' onclick = \"do_action('set_SSLclientKeysFile',SSLclientKeysFile.value);\"></td>");
			out.println(" </tr>");
			out.println(" <tr>");
			out.println("  <td>SSLkeyStorePass:</td>");
			out.println("  <td><input type = password id = 'SSLkeyStorePass' value = '" + server.currentConfig.getSSLkeyStorePass() + "'></td>");
			out.println("  <td><input type = button value = 'set' onclick = \"do_action('set_SSLkeyStorePass',SSLkeyStorePass.value);\"></td>");
			out.println(" </tr>");
			out.println(" <tr>");
			out.println("  <td>SSLkeyPassword:</td>");
			out.println("  <td><input type = password id = 'SSLkeyPassword' value = '" + server.currentConfig.getSSLkeyPassword() + "'></td>");
			out.println("  <td><input type = button value = 'set' onclick = \"do_action('set_SSLkeyPassword',SSLkeyPassword.value);\"></td>");
			out.println(" </tr>");
		}
		out.println(" <tr>");
		out.println("  <td>javaCompiler:</td>");
		out.println("  <td><input type = text id = 'javaCompiler' value = '" + server.currentConfig.getJavaCompiler() + "'></td>");
		out.println("  <td><input type = button value = 'set' onclick = \"do_action('set_javaCompiler',javaCompiler.value);\">");
		out.println("  <input type = button value = 'autoSet' onclick = \"do_action('set_javaCompiler','');\"></td>");
		out.println(" </tr>");
		out.println(" <tr>");
		out.println("  <td>defaultEncoding:</td>");
		out.println("  <td><input type = text id = 'defaultEncoding' value = '" + server.currentConfig.getDefaultEncoding() + "'></td>");
		out.println("  <td><input type = button value = 'set' onclick = \"do_action('set_defaultEncoding',defaultEncoding.value);\"></td>");
		out.println(" </tr>");
		out.println(" ");
		out.println(" <tr>");
		out.println("  <td>sessionExpires(in ms):</td>");
		out.println("  <td><input type = text id = 'sessionExpires' value = '" + server.currentConfig.getSessionExpires() + "'></td>");
		out.println("  <td><input type = button value = 'set' onclick = \"do_action('set_sessionExpires',sessionExpires.value);\"></td>");
		out.println(" </tr>");
		out.println(" ");
		out.println(" <tr>");
		out.println("  <td>configPageEnabled:</td>");
		out.println("  <td>" + getPullDown("configPageEnabled", server.currentConfig.getConfigPageEnabled()) + "</td>");
		out.println("  <td><input type = button value = 'set' onclick = \"do_action('set_configPageEnabled',configPageEnabled.value);\"></td>");
		out.println(" </tr>");
		out.println(" ");
		out.println(" <tr>");
		out.println("  <td>configPageURL:</td>");
		out.println("  <td><input type = text id = 'configPage' value = '" + server.currentConfig.getConfigPageUrl() + "'></td>");
		out.println("  <td><input type = button value = 'set' onclick = \"do_action('set_configPage',configPage.value);\"></td>");
		out.println(" </tr>");
		out.println(" <tr>");
		out.println("  <td colspan = 3>");
		out.println("  	config accessable IP.");
		out.println("		<table border=\"1\" width = 100%>");
		out.println("		 <tr>");
		out.println("		  <td>IP/HOSTNAME</td>");
		out.println("		  <td align=\"right\">operation</td>");
		out.println("		 </tr>");
		out.println("		");
		out.println("		 <tr>");
		out.println("		  <td><input type = text id = 'allowConfigIPs' value = ''></td>");
		out.println("		  <td align=\"right\"><input type = button value = 'add' onclick = \"do_action('add_allowConfigIPs',allowConfigIPs.value);\"></td>");
		out.println("		 </tr>");
		out.println("		</table>");
		out.println("  </td>");
		out.println(" </tr>");
		out.println(" <tr>");
		out.println("  <td>config admin userName:</td>");
		out.println("  <td><input type = text id = 'configUser' value = '" + server.currentConfig.getConfigUserName() + "'></td>");
		out.println("  <td>&nbsp;</td>");
		out.println(" </tr>");
		out.println(" <tr>");
		out.println("  <td>config admin old password:</td>");
		out.println("  <td><input type = password id = 'configPass1' value = ''></td>");
		out.println("  <td>&nbsp;</td>");
		out.println(" </tr>");
		out.println(" <tr>");
		out.println("  <td>config admin new password:</td>");
		out.println("  <td><input type = password id = 'configPass2' value = ''></td>");
		out.println("  <td>&nbsp;</td>");
		out.println(" </tr>");
		out.println(" <tr>");
		out.println("  <td>config admin new password confirm:</td>");
		out.println("  <td><input type = password id = 'configPass3' value = ''></td>");
		out.println("  <td><input type = button value = 'set' onclick = \"do_action('set_configPass',configPass1.value,configPass2.value,configPass3.value,configUser.value);\"></td>");
		out.println(" </tr>");
		out.println("</table>");
		out.println("File System Setting:");
		out.println("<table border=\"1\">");
		out.println(" <tr>");
		out.println("  <td>url</td>");
		out.println("  <td>rootPath</td>");
		out.println("  <td>allowList</td>");
		out.println("  <td>allowUpLoad</td>");
		out.println("  <td>allowDel</td>");
		out.println("  <td>isDownload</td>");
		out.println("  <td>userName</td>");
		out.println("  <td>passWord</td>");
		out.println("  <td>operation</td>");
		out.println(" </tr>");

		Integer n = (Integer) session.getPrivate("edit_fileSystem");
		for (int i = 0; i < server.currentConfig.getFileSystems().size(); i++)
		{
			ServerConfig.FileSystem cfs = server.currentConfig.getFileSystems().get(i);
			if (n != null && n.intValue() == i)
			{
				out.println(" <tr>");
				out.println("  <td><input type = text id = 'url' value = '" + cfs.url + "'></td>");
				out.println("  <td><input type = text id = 'rootPath' value = '" + cfs.rootPath + "'></td>");
				out.println("  <td>" + getPullDown("allowList", cfs.allowList) + "</td>");
				out.println("  <td>" + getPullDown("allowUpLoad", cfs.allowUpLoad) + "</td>");
				out.println("  <td>" + getPullDown("allowDel", cfs.allowDel) + "</td>");
				out.println("  <td>" + getPullDown("isDownload", cfs.isDownload) + "</td>");
				out.println("  <td><input type = text id = 'userName' value = '" + (cfs.userName == null ? "" : cfs.userName) + "'></td>");
				out.println("  <td><input type = password id = 'passWord' value = ''></td>");
				out.println("  <td><input type = button value = 'update' onclick = \"do_action('upd_fileSystem',url.value,rootPath.value,allowList.value,allowUpLoad.value,allowDel.value,isDownload.value,passWord.value,userName.value);\"></td>");
				out.println(" </tr>");
			}
			else
			{
				out.println(" <tr>");
				out.println("  <td><a href = '" + cfs.url + "'>" + cfs.url + "</a></td>");
				out.println("  <td>" + cfs.rootPath + "</td>");
				out.println("  <td>" + cfs.allowList + "</td>");
				out.println("  <td>" + cfs.allowUpLoad + "</td>");
				out.println("  <td>" + cfs.allowDel + "</td>");
				out.println("  <td>" + cfs.isDownload + "</td>");
				out.println("  <td>" + (cfs.userName == null || "".equals(cfs.userName) ? "&nbsp;" : cfs.userName) + "</td>");
				out.println("  <td>" + (cfs.passWord == null ? "&nbsp;" : "******") + "</td>");
				out.println("  <td>");
				out.println("   <input type = button value = 'edit' onclick = \"do_action('edit_fileSystem'," + i + ");\">");
				out.println("   <input type = button value = 'del' onclick = \"do_action('del_fileSystem'," + i + ");\">");
				out.println("  </td>");
				out.println(" </tr>");
			}
		}

		if (n == null)
		{
			out.println(" <tr>");
			out.println("  <td><input type = text id = 'url' value = ''></td>");
			out.println("  <td><input type = text id = 'rootPath' value = ''></td>");
			out.println("  <td>" + getPullDown("allowList", false) + "</td>");
			out.println("  <td>" + getPullDown("allowUpLoad", false) + "</td>");
			out.println("  <td>" + getPullDown("allowDel", false) + "</td>");
			out.println("  <td>" + getPullDown("isDownload", false) + "</td>");
			out.println("  <td><input type = text id = 'userName' value = ''></td>");
			out.println("  <td><input type = password id = 'passWord' value = ''></td>");
			out.println("  <td><input type = button value = 'add' onclick = \"do_action('add_fileSystem',url.value,rootPath.value,allowList.value,allowUpLoad.value,allowDel.value,passWord.value,userName.value);\"></td>");
			out.println(" </tr>");
		}
		out.println("</table>");
		out.println("</table>");
		out.println("JSP Setting:");
		out.println("<table border=\"1\">");
		out.println(" <tr>");
		out.println("  <td>url</td>");
		out.println("  <td>rootPath</td>");
		out.println("  <td>operation</td>");
		out.println(" </tr>");

		n = (Integer) session.getPrivate("edit_jspProcessors");
		for (int i = 0; i < server.currentConfig.getJspProcessors().size(); i++)
		{
			ServerConfig.JspProcessor cjp = server.currentConfig.getJspProcessors().get(i);
			if (n != null && n.intValue() == i)
			{
				out.println(" <tr>");
				out.println("  <td><input type = text id = 'JSPurl' value = '" + cjp.url + "'></td>");
				out.println("  <td><input type = text id = 'JSProotPath' value = '" + cjp.rootPath + "'></td>");
				out.println("  <td><input type = button value = 'update' onclick = \"do_action('upd_jspProcessors',JSPurl.value,JSProotPath.value);\"></td>");
				out.println(" </tr>");
			}
			else
			{
				out.println(" <tr>");
				out.println("  <td><a href = '" + cjp.url + "'>" + cjp.url + "</a></td>");
				out.println("  <td>" + cjp.rootPath + "</td>");
				out.println("  <td>");
				out.println("   <input type = button value = 'edit' onclick = \"do_action('edit_jspProcessors'," + i + ");\">");
				out.println("   <input type = button value = 'del' onclick = \"do_action('del_jspProcessors'," + i + ");\">");
				out.println("  </td>");
				out.println(" </tr>");
			}
		}

		if (n == null)
		{
			out.println(" <tr>");
			out.println("  <td><input type = text id = 'JSPurl' value = ''></td>");
			out.println("  <td><input type = text id = 'JSProotPath' value = ''></td>");
			out.println("  <td><input type = button value = 'add' onclick = \"do_action('add_jspProcessors',JSPurl.value,JSProotPath.value);\"></td>");
			out.println(" </tr>");
		}
		out.println("</table>");
		out.println("</table>");
		out.println("servlet Setting:");
		out.println("<table border=\"1\">");
		out.println(" <tr>");
		out.println("  <td>url</td>");
		out.println("  <td>class</td>");
		out.println("  <td>isSingleton</td>");
		out.println("  <td>operation</td>");
		out.println(" </tr>");

		n = (Integer) session.getPrivate("edit_servlets");
		for (int i = 0; i < server.currentConfig.getServlets().size(); i++)
		{
			ServerConfig.Servlet cs = server.currentConfig.getServlets().get(i);
			if (n != null && n.intValue() == i)
			{
				out.println(" <tr>");
				out.println("  <td><input type = text id = 'ServletUrl' value = '" + cs.url + "'></td>");
				out.println("  <td><input type = text id = 'class_name' value = '" + cs.classFileName + "'></td>");
				out.println("  <td>" + getPullDown("isSingleton", cs.isSingleton) + "</td>");
				out.println("  <td><input type = button value = 'update' onclick = \"do_action('upd_servlets',ServletUrl.value,class_name.value,isSingleton.value);\"></td>");
				out.println(" </tr>");
			}
			else
			{
				out.println(" <tr>");
				out.println("  <td><a href = '" + cs.url + "'>" + cs.url + "</a></td>");
				out.println("  <td>" + cs.classFileName + "</td>");
				out.println("  <td>" + cs.isSingleton + "</td>");
				out.println("  <td>");
				out.println("   <input type = button value = 'edit' onclick = \"do_action('edit_servlets'," + i + ");\">");
				out.println("   <input type = button value = 'del' onclick = \"do_action('del_servlets'," + i + ");\">");
				out.println("  </td>");
				out.println(" </tr>");
			}
		}

		if (n == null)
		{
			out.println(" <tr>");
			out.println("  <td><input type = text id = 'ServletUrl' value = ''></td>");
			out.println("  <td><input type = text id = 'class_name' value = ''></td>");
			out.println("  <td>" + getPullDown("isSingleton", false) + "</td>");
			out.println("  <td><input type = button value = 'add' onclick = \"do_action('add_servlets',ServletUrl.value,class_name.value,isSingleton.value);\"></td>");
			out.println(" </tr>");
		}
		out.println("</table>");
		out.println("</body>");
		out.println("</html>");

	}

	private static void makeStatus(Request req, Response res, Server server) throws Exception
	{
		int ts_cnt = Thread.activeCount();
		Thread[] ts = new Thread[ts_cnt];
		Thread.enumerate(ts);
		PrintWriter out = res.getWriter();

		synchronized (server.sessionPooling)
		{
			Object[] os = server.sessionPooling.keySet().toArray();
			out.println("<html>");
			out.println("<body>");
			out.println("threads:<br>");
			for (int i = 0; i < ts.length; i++)
			{
				out.println(ts[i].getName() + "<br>");
			}
			out.println("sessions:<br>");
			for (int i = 0; i < os.length; i++)
			{
				Session s = server.getSession((String) os[i]);
				out.println(s.getId() + "<br>");
			}
		}
		out.println("</body>");
		out.println("</html>");
	}

	private static void makeMsg(Request req, Response res, Server server, String msg) throws Exception
	{
		PrintWriter out = res.getWriter();
		out.println("<html>");
		out.println("<body>");
		out.println(msg);
		out.println("<br>");
		out.println("<a href=" + server.currentConfig.getConfigPageUrl() + ">return</a>");
		out.println("</body>");
		out.println("</html>");
	}

	private static void makeLogin(Request req, Response res, Server server) throws Exception
	{
		PrintWriter out = res.getWriter();
		out.println("<html>");
		out.println("<body>");
		out.println("<form name = 'formMain' action = '" + server.currentConfig.getConfigPageUrl() + "' method = 'post'>");
		out.println("<input type = \"hidden\" name = \"action\" id = \"action\" value=\"login\">");
		out.println("username:<input type = \"text\" name = \"username\"  id = \"username\"><br>");
		out.println("password:<input type = \"password\" name = \"password\" id = \"password\"><br>");
		out.println("<input type = \"submit\"><br>");
		out.println("</form>");
		out.println("</body>");
		out.println("</html>");
	}

}
