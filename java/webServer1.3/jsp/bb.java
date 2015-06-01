import java.io.PrintStream;
import webServer.JspPage;
import webServer.Request;
import webServer.Response;
import webServer.Session;
import webServer.Server;

public class bb extends JspPage {
  public boolean authorization(String username, String password){
  return true;
	 }

	public void doRequest() throws Exception{

sample.TagClass t = new sample.TagClass();

String s = t.getStr();

out.print(s);

	}
}
