package sample;

import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.Transferable;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;
import webServer.Session;

public class ClipBoardAction extends Servlet
{
	@Override
	public void doReq(Request req, Response res, Server server) throws Exception
	{
		if (!"T".equals(req.getSession().get(LoginServlet.LOGIN))) {
			res.sendRedirect("Login");
			return;
		}

		String val = req.getPOSTParam("val");
		try
		{
			Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
			Transferable tText = new StringSelection(val);
			clipboard.setContents(tText, null);
		}
		catch (Exception e)
		{
			//
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
