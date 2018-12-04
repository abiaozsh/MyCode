package sample;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;

public class ProcessKillAction extends Servlet {
	@Override
	public void doReq(Request req, Response res, Server server) throws Exception {
		if (!"T".equals(req.getSession().get(LoginServlet.LOGIN))) {
			res.sendRedirect("Login");
			return;
		}

		String pid = req.getGETParam("pid");
		String flg = req.getGETParam("flg");

		if ("ntsd".equals(flg)) {
			Runtime.getRuntime().exec("ntsd -c q -p " + pid);
		} else {
			Runtime.getRuntime().exec("tskill " + pid);
		}
	}
}
