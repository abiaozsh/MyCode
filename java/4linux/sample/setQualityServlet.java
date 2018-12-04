package sample;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;
import webServer.Session;

public class setQualityServlet extends Servlet
{
	public String getPath()
	{
		return "/setQuality";
	}

	@Override
	public void doReq(Request req, Response res, Server server) throws Exception
	{
		Session session = req.getSession();
		if (!"T".equals(session.get(LoginServlet.LOGIN))) {
			res.sendRedirect("Login");
			return;
		}

		String quality = req.getPOSTParam("quality");
		if (quality != null)
		{
			session.set("quality", quality);
		}
		Session s = req.getSession();
		if (s.get("Redirect") != null)
		{
			res.sendRedirect((String) s.get("Redirect"));
		}
		else
		{
			res.sendRedirect("/jsp/control.jsp");
		}
	}
}
