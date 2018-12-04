<%
//jsp available: PrintStream out, Request request, Session session, Server server
%>

<html>
<body>
<form name = 'formMain' action = '/UploadServlet' method = 'post' enctype='multipart/form-data'>
<input type = text name = 'text'>
<input type = file name = 'file1'>
<input type = file name = 'file2'>
<input type = submit>
</form>
<a href="/">back</a>
</body>
</html>
