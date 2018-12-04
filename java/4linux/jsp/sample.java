import java.io.PrintStream;
import webServer.JspPage;
import webServer.Request;
import webServer.Response;
import webServer.Session;
import webServer.Server;
import java.util.*;

public class sample extends JspPage {
  public boolean authorization(String username, String password){
  return true;
	 }

private int testFunction(int a)
{
	return a+a;
}



	public void doRequest() throws Exception{

//jsp available: PrintStream out, Request request, Session session, Server server
out.println("<html>");
out.println("sample.jsp");
out.print(testFunction(2));
out.println("<br>");

response.addCookie("name2", "value3", 1000, "");

for(int i=0;i<10;i++)
{
	out.print(i);
}
String sv = (String)session.get("sessionValue");
out.println("<br>");
out.println("Cookies:<br>");
out.print("value:");
out.print(request.getCookie("name"));
out.println("<br>");
out.print("value1:");
out.print(request.getCookie("name1"));
out.println("<br>");
out.print("value2:");
out.print(request.getCookie("name2"));
out.println("<br>");
out.println("<br>");
out.print("sessionValue:");
out.print(sv);
out.println("\\\"");
out.println("</html>");

	}
}
