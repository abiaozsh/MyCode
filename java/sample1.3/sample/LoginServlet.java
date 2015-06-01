package sample;

import java.io.PrintWriter;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;
import webServer.Session;

public class LoginServlet extends Servlet {
	public final static String LOGIN = "SessionLogin";

	private boolean login(String username, String password) {
		if ("zhengsh".equals(username) && "zhengsh".equals(password)) {
			return true;
		} else {
			return false;
		}
	}

	@Override
	public void doReq(Request req, Response res, Server server) throws Exception {
		Session session = req.getSession();
		String action = req.getPOSTParam("action");

		if ("login".equals(action)) {
			boolean pass = login(req.getPOSTParam("username"), req.getPOSTParam("password"));

			if (pass) {
				session.set(LOGIN, "T");
				res.sendRedirect("Control");
				return;
			} else {
				return;
			}
		} else if ("logout".equals(action)) {
			session.set(LOGIN, null);
			makeLogin(req, res, server);
			return;
		} else {
			if (!"T".equals(session.get(LOGIN))) {
				makeLogin(req, res, server);
				return;
			}
		}

	}

	private static void makeLogin(Request req, Response res, Server server) throws Exception {
		PrintWriter out = res.getWriter();
		out.println("<html>");
		out.println("<body>");
		out.println("<form name = 'formMain' method = 'post'>");
		out.println("<input type = \"hidden\" name = \"action\" id = \"action\" value=\"login\">");
		out.println("username:<input type = \"text\" name = \"username\"  id = \"username\"><br>");
		out.println("password:<input type = \"password\" name = \"password\" id = \"password\"><br>");
		out.println("<input type = \"submit\"><br>");
		out.println("</form>");
		out.println("</body>");
		out.println("</html>");
	}

}
