import java.io.PrintStream;
import webServer.JspPage;
import webServer.Request;
import webServer.Session;
import webServer.Server;
public class msg extends JspPage {
	public void doRequest() throws Exception{

String msg = (String)session.getPrivate("config_message");
session.removePrivate("config_message");
out.print("<html>\r\n");
out.print("<body>\r\n");
out.print(msg);
out.print("<br>\r\n");
out.print("<a href=");
out.print(server.currentConfig.configPage);
out.print(">return</a>\r\n");
out.print("</body>\r\n");
out.print("</html>\r\n");
	}
}
