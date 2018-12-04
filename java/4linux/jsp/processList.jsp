<%@ page import="java.util.*" %>
<html>
<table border="1">

<%
ArrayList<String[]> al = (ArrayList<String[]>)response.getAttribute("al");

for(String[] ss : al)
{%>
<tr>
<td>
<a href="/processKill?pid=<%=ss[1]%>">kill<%=ss[1]%></a> <a href="/processKill?pid=<%=ss[1]%>&flg=ntsd">ntsd<%=ss[1]%></a>
</td>
<%for(String s : ss){%>
<td>
<%=s%><br>
</td>
<%}%>
</tr>
<%}%>
			//"图像名    ","PID","会话名      ","会话#   ","内存使用 ","状态  ","用户名   ","C
			//PU 时间","窗口标题    "
</html>