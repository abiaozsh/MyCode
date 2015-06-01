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
		String quality = req.getPOSTParam("quality");
		if (quality != null)
		{
			session.set("quality", quality);
		}
		res.sendRedirect("/jsp/control.jsp");
	}
}
