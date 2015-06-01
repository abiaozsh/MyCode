import java.io.PrintStream;
import webServer.JspPage;
import webServer.Request;
import webServer.Response;
import webServer.Session;
import webServer.Server;
public class aa extends JspPage {
  public boolean authorization(String username, String password){  return true;	 }
	public void doRequest() throws Exception{
out.println("<html>");
out.println("<table border=1>");

int n1=0x59;
for(int i=0;i<15;i++)
{
out.println("<tr>");

for(int j=0;j<15;j++)
{
int val = n1*0x0100+i*0x10+j;
String s = new String(new char[]{(char)val});
String v = Integer.toString(val,16);
if(val<0x10)
{
	v="000"+v;
}
else if(val<0x100)
{
	v="00"+v;
}
else if(val<0x1000)
{
	v="0"+v;
}
v=v.toUpperCase();
out.println("<td>");
out.print("<img src=\"http://www.decodeunicode.org/data/glyph/26x26/");
out.print(v);
out.print(".gif\">");
out.print(s);
out.print("(");
out.print(v);
out.println(")");
out.println("</td>");
}out.println("</tr>");
}out.println("</table>");
out.println("</html>");
	}
}
