package sample;

import java.io.PrintWriter;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;

public class Root extends Servlet
{
	@Override
	public void doReq(Request req, Response res, Server server) throws Exception
	{
		PrintWriter out = res.getWriter();
		String i = (String) req.getSession().get("i");
		if (i == null)
		{
			i = "";
		}
		i = i + "1";

		req.getSession().set("i", i);
		out.println("<html>");
		out.println("<body>");
		out.println("<a href = '/jsp/control.jsp'>servlet jsp sample</a><br>");
		out.println("<a href = '/Control'>servlet sample</a><br>");
		out.println("<a href = '/file/'>download sample</a><br>");
		out.println("<a href = '/jsp/upload.jsp'>upload sample</a><br>");
		out.println("<a href = '/Sample'>cookie sample</a><br>");
		out.println("<a href = '/config'>config</a><br>");
		out.println("idx:" + i + "<br>");
		out.println("</body>");
		out.println("</html>");
	}
}
