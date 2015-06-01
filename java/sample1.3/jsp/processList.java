import java.io.PrintStream;
import webServer.JspPage;
import webServer.Request;
import webServer.Response;
import webServer.Session;
import webServer.Server;
import java.util.*;

public class processList extends JspPage {
  public boolean authorization(String username, String password){
  return true;
	 }

	public void doRequest() throws Exception{
out.println("<html>");
out.println("<table border=\"1\">");

ArrayList<String[]> al = (ArrayList<String[]>)response.getAttribute("al");

for(String[] ss : al)
{out.println("<tr>");
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
for(String s : ss){out.println("<td>");
out.print(s);
out.println("<br>");
out.println("</td>");
}out.println("</tr>");
}out.println("			//\"图像名    \",\"PID\",\"会话名      \",\"会话#   \",\"内存使用 \",\"状态  \",\"用户名   \",\"C");
out.println("			//PU 时间\",\"窗口标题    \"");
out.println("</html>");

	}
}
