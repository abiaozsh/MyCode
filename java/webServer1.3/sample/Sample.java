package sample;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;
import webServer.Session;

public class Sample extends Servlet
{
	@Override
	public void doReq(Request req, Response res, Server server) throws Exception
	{
		Session session = req.getSession();
		session.set("sessionValue", "fdsa");
		System.out.print(req.getCookie("name") + "\n");
		System.out.print(req.getCookie("name1") + "\n");
		System.out.print(req.getCookie("name2") + "\n");
		res.addCookie("name", "value", 1000, "");
		res.addCookie("name1", "value1", 1000, "");
		server.getRequestDispatcher("/jsp/sample.jsp").forward(req, res);
	}
}
