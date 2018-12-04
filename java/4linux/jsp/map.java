import java.io.PrintStream;
import webServer.JspPage;
import webServer.Request;
import webServer.Response;
import webServer.Session;
import webServer.Server;

public class map extends JspPage {
  public boolean authorization(String username, String password){
  return true;
	 }

	public void doRequest() throws Exception{

	String player = request.getPOSTParam("player");
	String group = request.getPOSTParam("group");
	session.set("player",player);
	session.set("group",group);
out.println("<html>");
out.println("<head>");
out.println("<script>");
out.println("function mouseDown(){");
out.println(" var x,y;");
out.println(" x=document.body.scrollLeft+event.clientX;");
out.println(" y=document.body.scrollTop +event.clientY;");
out.println(" formMain.x.value = x-12-401;");
out.println(" formMain.y.value = y-17-401;");
out.println(" //formMain.submit();");
out.println("}");
out.println("</script>");
out.println("</head>");
out.println("<body>");
out.println("<img src = '/map' onclick = 'mouseDown()'>");
out.println("<form name = 'formMain' action=\"/jsp/map.jsp\" method=\"post\">");
out.println("x:<input type = text name = 'y'>");
out.println("y:<input type = text name = 'x'>");
out.println("player:<input type = text name = 'player'>");
out.println("group:<input type = text name = 'group'>");
out.println("<input type = submit>");
out.println("</form>");
out.println("</body>");
out.println("</html>");

	}
}
