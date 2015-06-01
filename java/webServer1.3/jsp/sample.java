import java.io.PrintStream;
import webServer.JspPage;
import webServer.Request;
import webServer.Response;
import webServer.Session;
import webServer.Server;
import java.util.*;

public class sample extends JspPage {
  public boolean authorization(String username, String password){

return "abc".equals(username) && "abc".equals(password);

	 }

private String sample()
{
	out.print("aa\n");
	out.print(request.getCookie("name")+"\n");
	return "bb";
}


	public void doRequest() throws Exception{
out.println("<html>");
out.println("sample.jsp");
out.println("<br>");
out.println("if(request.getCookie(\"na\\\"m\\\\ \\\\\\\\ \\\"\\\" \\\\\" \\\\\\\"e\")==null)");

if(request.getCookie("name")==null)
{
	response.addCookie("name", "value", 1000, "");
}
else
{
	response.removeCookie("name", "");
}

response.addCookie("name2", "value2", 1000, "");
String sv = (String)session.get("sessionValue");

out.print(request.getCookie("name")+"\n");
out.print(request.getCookie("name1")+"\n");
out.print(request.getCookie("name2")+"\n");
out.print(sample());

out.println("<br>");
out.print("sessionValue:");
out.print(sv+"fd\nsa");
out.println("</html>");

	}
}
