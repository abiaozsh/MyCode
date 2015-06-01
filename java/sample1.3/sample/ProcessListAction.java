package sample;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;

public class ProcessListAction extends Servlet {
	@Override
	public void doReq(Request req, Response res, Server server) throws Exception {
		if (!"T".equals(req.getSession().get(LoginServlet.LOGIN))) {
			res.sendRedirect("Login");
			return;
		}

		Process process = Runtime.getRuntime().exec("tasklist /fo:csv /v");

		Scanner in = new Scanner(process.getInputStream());
		ArrayList<String[]> al = new ArrayList<String[]>();
		while (in.hasNextLine()) {
			String line = in.nextLine();
			if (!"".equals(line) && !line.startsWith("\"图像名")) {
				String[] datas = line.split("\",\"");
				al.add(datas);
			}
		}
		PrintWriter out = res.getWriter();

		out.println("<html>");
		out.println("<table border=\"1\">");

		for (String[] ss : al) {
			if (ss.length > 0) {
				out.println("<tr>");
				out.println("<td>");
				out.print("<a href=\"/processKill?pid=");
				out.print(ss[1]);
				out.print("\">kill");
				out.print(ss[1]);
				out.print("</a> <a href=\"/processKill?pid=");
				out.print(ss[1]);
				out.print("&flg=ntsd\">ntsd");
				out.print(ss[1]);
				out.println("</a>");
				out.println("</td>");
				for (String s : ss) {
					out.println("<td>");
					out.print(s);
					out.println("<br>");
					out.println("</td>");
				}
				out.println("</tr>");
			}
		}
		out.println("</html>");

	}
}
