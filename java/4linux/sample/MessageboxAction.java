package sample;
import javax.swing.JOptionPane;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;

public class MessageboxAction extends Servlet
{
	@Override
	public void doReq(Request req, Response res, Server server) throws Exception
	{
		if (!"T".equals(req.getSession().get(LoginServlet.LOGIN))) {
			res.sendRedirect("Login");
			return;
		}

		String msg = req.getPOSTParam("msg");
		JOptionPane.showMessageDialog(null, msg, "title", JOptionPane.INFORMATION_MESSAGE);
		res.sendRedirect("/jsp/control.jsp");
	}
}
