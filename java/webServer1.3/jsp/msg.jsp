<%
String msg = (String)session.getPrivate("config_message");
session.removePrivate("config_message");
%>
<html>
<body>
<%=msg%><br>
<a href=<%=server.currentConfig.configPage%>>return</a>
</body>
</html>
