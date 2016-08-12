import java.io.PrintStream;
import webServer.JspPage;
import webServer.Request;
import webServer.Response;
import webServer.Session;
import webServer.Server;

public class index extends JspPage {
  public boolean authorization(String username, String password){
  return true;
	 }

	public void doRequest() throws Exception{
out.println("﻿<html>");
out.println("<head>");
out.println("</head>");
out.println("<body>");
out.println("<a href=\"tutorial1.jsp\">1:数值计算</a><br/>");
out.println("<a href=\"tutorial2.jsp\">2:变量</a><br/>");
out.println("<a href=\"tutorial3.jsp\">3:编程中的错误</a><br/>");
out.println("<a href=\"tutorial4.jsp\">4:表达式</a><br/>");
out.println("<a href=\"tutorial5.jsp\">5:函数</a><br/>");
out.println("</body>");
out.println("</html>");

	}
}
