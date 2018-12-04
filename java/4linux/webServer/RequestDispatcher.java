package webServer;

import java.io.InputStream;

public class RequestDispatcher
{
	String target;

	Server server;

	RequestDispatcher(String target, Server server)
	{
		this.target = target;
		this.server = server;
	}

	static InputStream is;
	static
	{
		try
		{
			is = ClassLoader.getSystemResourceAsStream("favicon.ico");
			if (is != null)
			{
				is.mark(0x7FFFFFFF);
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	public void forward(Request req, Response res)
	{
		try
		{
			// servlet

			// server icon
			if (is != null)
			{
				if ("/favicon.ico".equals(target))
				{
					is.reset();
					res.setInputStream(is, false);
					res.setContentType("image/x-icon");
					return;
				}
			}

			Servlet servlet = null;
			synchronized (server.servlets)
			{
				ServletPack sp = server.servlets.get(target);
				if (sp != null)
				{
					servlet = sp.getInstance();
				}
			}
			if (servlet != null)
			{
				if (!servlet.authorization(req.authorizationUsername, req.authorizationPassword, server))
				{
					res.unAuthenticated(servlet.url);
					res.closeConnection();
					return;
				}
				servlet.doReq(req, res, server);
				return;
			}

			// jsp
			synchronized (server.jspProcessors)
			{
				for (JspProcessor jsp : server.jspProcessors)
				{
					if (target.startsWith(jsp.getUrl()))
					{
						JspPage jspPage = jsp.getNewJspPage(target, server);
						if (jspPage != null)
						{
							if (!jspPage.authorization(req.authorizationUsername, req.authorizationPassword))
							{
								res.unAuthenticated(jspPage.url);
								res.closeConnection();
								return;
							}
							jspPage.doReq(req, res, server);
							return;
						}
					}
				}
			}

			// file
			synchronized (server.fileSystems)
			{
				for (FileSystem fs : server.fileSystems)
				{
					if (target.startsWith(fs.getUrl()))
					{
						//if (fs.isExist(target, server))
						{
							fs.doReq(target, req, res, server);
							return;
						}
					}
				}
			}
			res.notFound();
		}
		catch (Throwable e)
		{
			res.exceptionPage(e);
		}
	}
}
