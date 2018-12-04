<%
	String player = request.getPOSTParam("player");
	String group = request.getPOSTParam("group");
	session.set("player",player);
	session.set("group",group);
%>
<html>
<head>

<script>
function mouseDown(){
 var x,y;
 x=document.body.scrollLeft+event.clientX;
 y=document.body.scrollTop +event.clientY;
 formMain.x.value = x-12-401;
 formMain.y.value = y-17-401;
 //formMain.submit();
}
</script>

</head>
<body>
<img src = '/map' onclick = 'mouseDown()'>
<form name = 'formMain' action="/jsp/map.jsp" method="post">
x:<input type = text name = 'y'>
y:<input type = text name = 'x'>
player:<input type = text name = 'player'>
group:<input type = text name = 'group'>
<input type = submit>
</form>
</body>
</html>