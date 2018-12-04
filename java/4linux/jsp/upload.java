import java.io.PrintStream;
import webServer.JspPage;
import webServer.Request;
import webServer.Response;
import webServer.Session;
import webServer.Server;

public class upload extends JspPage {
  public boolean authorization(String username, String password){
  return true;
	 }

	public void doRequest() throws Exception{

//jsp available: PrintStream out, Request request, Session session, Server server
out.println("<html>");
out.println("<body>");
out.println("<form name = 'formMain' action = '/UploadServlet' method = 'post' enctype='multipart/form-data'>");
out.println("<input type = text name = 'text'>");
out.println("<input type = file name = 'file1'>");
out.println("<input type = file name = 'file2'>");
out.println("<input type = submit>");
out.println("</form>");
out.println("<a href=\"/\">back</a>");
out.println("</body>");
out.println("</html>");

	}
}
