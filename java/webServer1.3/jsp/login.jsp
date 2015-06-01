<html>
<body>
welcome to mini web Server<br>
input administrator password<br>
<form action = '<%=server.currentConfig.configPage%>' method = 'post'>
<input type = hidden name = 'action' value = 'login'>
<input type = password name = 'pass'>
<input type = submit>
</form>
</body>
</html>
