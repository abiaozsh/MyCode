<%@ page import="java.util.*" %>
<%@ authorization
return "abc".equals(username) && "abc".equals(password);
%>
<html>
sample.jsp
<br>
if(request.getCookie("na\"m\\ \\\\ \"\" \\" \\\"e")==null)
<%
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
%>
<%
out.print(request.getCookie("name")+"\n");
out.print(request.getCookie("name1")+"\n");
out.print(request.getCookie("name2")+"\n");
out.print(sample());

%>
<br>
sessionValue:<%=sv+"fd\nsa"%>
</html>


<%!
private String sample()
{
	out.print("aa\n");
	out.print(request.getCookie("name")+"\n");
	return "bb";
}
%>
