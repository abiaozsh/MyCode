import java.io.PrintStream;
import webServer.JspPage;
import webServer.Request;
import webServer.Session;
import webServer.Server;
public class login extends JspPage {
	public void doRequest() throws Exception{
out.print("<html>\r\n");
out.print("<body>\r\n");
out.print("welcome to mini web Server<br>\r\n");
out.print("input administrator password<br>\r\n");
out.print("<form action = '");
out.print(server.currentConfig.configPage);
out.print("' method = 'post'>\r\n");
out.print("<input type = hidden name = 'action' value = 'login'>\r\n");
out.print("<input type = password name = 'pass'>\r\n");
out.print("<input type = submit>\r\n");
out.print("</form>\r\n");
out.print("</body>\r\n");
out.print("</html>\r\n");
	}
}
