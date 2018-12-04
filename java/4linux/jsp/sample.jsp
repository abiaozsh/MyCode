<%@ page import="java.util.*" %>
<%
//jsp available: PrintStream out, Request request, Session session, Server server
%>
<%!
private int testFunction(int a)
{
	return a+a;
}

%>


<html>
sample.jsp
<%=testFunction(2)%>
<br>
<%
response.addCookie("name2", "value3", 1000, "");

for(int i=0;i<10;i++)
{
	out.print(i);
}
String sv = (String)session.get("sessionValue");
%>
<br>
Cookies:<br>
value:<%=request.getCookie("name")%><br>
value1:<%=request.getCookie("name1")%><br>
value2:<%=request.getCookie("name2")%><br>
<br>
sessionValue:<%=sv%>

\"
</html>
