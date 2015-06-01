<%@ authorization
if("zhengsh".equals(username) && "zhengsh".equals(password))
{
	return true;
}
else
{
	return false;
}
%>
<%
//jsp available: PrintStream out, Request request, Session session, Server server
		session.set("pass","ok");

%>
<html>
<head>
<title>title</title></head>
<body onload = 'document.onkeydown = keyDown;'>
<script type="text/javascript">
var posx;var posy;
function getMouse(e){
	posx=0;posy=0;
	var ev=(!e)?window.event:e;//IE:Moz
	if (ev.pageX){//Moz
		posx=ev.pageX;//+window.pageXOffset;
		posy=ev.pageY;//+window.pageYOffset;
		document.formMain.x.value = posx-8;
		document.formMain.y.value = posy-8;
	}
	else if(ev.clientX){//IE
		posx=ev.clientX+document.body.scrollLeft;
		posy=ev.clientY+document.body.scrollTop;
		document.formMain.x.value = posx-12;
		document.formMain.y.value = posy-17;
	}
	else{return false}//old browsers
}
document.onmousemove=getMouse
</script>

<script>
function keyDown()
{
 var k = event.keyCode;
 if(k==17)formMain.action.value = 2;
 else if(k==16)formMain.action.value = 3;
}
function mouseDown(){
 document.formMain.submit();
}
</script>
<img src='/Screen' onclick = 'mouseDown()'>

<form name = 'formMain' action = '/MouseAction' method = 'post'>
<select name = 'action'>
<option value = '1'>single click</option>
<option value = '2'>double click</option>
<option value = '3'>right click</option>
<option value = '4'>drag</option>
<option value = '5'>drop</option>
</select>
<input type = text name = 'x'>
<input type = text name = 'y'>
</form>

<form name = 'formKey' action = '/KeyAction' method = 'post'>
<select name = 'key1'>
<option value = ''>NONE</option>
<option value = 'VK_SHIFT'>VK_SHIFT</option>
<option value = 'VK_CONTROL'>VK_CONTROL</option>
<option value = 'VK_ALT'>VK_ALT</option>
<option value = 'VK_WINDOWS'>VK_WINDOWS</option>
</select>
<select name = 'key2'>
<option value = ''>NONE</option>
<option value = 'VK_WINDOWS'>VK_WINDOWS</option>
<option value = 'VK_CONTEXT_MENU'>VK_CONTEXT_MENU</option>
<option value = 'VK_ENTER'>VK_ENTER</option>
<option value = 'VK_BACK_SPACE'>VK_BACK_SPACE</option>
<option value = 'VK_TAB'>VK_TAB</option>
<option value = 'VK_PAUSE'>VK_PAUSE</option>
<option value = 'VK_CAPS_LOCK'>VK_CAPS_LOCK</option>
<option value = 'VK_ESCAPE'>VK_ESCAPE</option>
<option value = 'VK_SPACE'>VK_SPACE</option>
<option value = 'VK_PAGE_UP'>VK_PAGE_UP</option>
<option value = 'VK_PAGE_DOWN'>VK_PAGE_DOWN</option>
<option value = 'VK_END'>VK_END</option>
<option value = 'VK_HOME'>VK_HOME</option>
<option value = 'VK_LEFT'>VK_LEFT</option>
<option value = 'VK_UP'>VK_UP</option>
<option value = 'VK_RIGHT'>VK_RIGHT</option>
<option value = 'VK_DOWN'>VK_DOWN</option>
<option value = 'VK_COMMA'>VK_COMMA</option>
<option value = 'VK_MINUS'>VK_MINUS</option>
<option value = 'VK_PERIOD'>VK_PERIOD</option>
<option value = 'VK_SLASH'>VK_SLASH</option>
<option value = 'VK_0'>VK_0</option>
<option value = 'VK_1'>VK_1</option>
<option value = 'VK_2'>VK_2</option>
<option value = 'VK_3'>VK_3</option>
<option value = 'VK_4'>VK_4</option>
<option value = 'VK_5'>VK_5</option>
<option value = 'VK_6'>VK_6</option>
<option value = 'VK_7'>VK_7</option>
<option value = 'VK_8'>VK_8</option>
<option value = 'VK_9'>VK_9</option>
<option value = 'VK_SEMICOLON'>VK_SEMICOLON</option>
<option value = 'VK_EQUALS'>VK_EQUALS</option>
<option value = 'VK_A'>VK_A</option>
<option value = 'VK_B'>VK_B</option>
<option value = 'VK_C'>VK_C</option>
<option value = 'VK_D'>VK_D</option>
<option value = 'VK_E'>VK_E</option>
<option value = 'VK_F'>VK_F</option>
<option value = 'VK_G'>VK_G</option>
<option value = 'VK_H'>VK_H</option>
<option value = 'VK_I'>VK_I</option>
<option value = 'VK_J'>VK_J</option>
<option value = 'VK_K'>VK_K</option>
<option value = 'VK_L'>VK_L</option>
<option value = 'VK_M'>VK_M</option>
<option value = 'VK_N'>VK_N</option>
<option value = 'VK_O'>VK_O</option>
<option value = 'VK_P'>VK_P</option>
<option value = 'VK_Q'>VK_Q</option>
<option value = 'VK_R'>VK_R</option>
<option value = 'VK_S'>VK_S</option>
<option value = 'VK_T'>VK_T</option>
<option value = 'VK_U'>VK_U</option>
<option value = 'VK_V'>VK_V</option>
<option value = 'VK_W'>VK_W</option>
<option value = 'VK_X'>VK_X</option>
<option value = 'VK_Y'>VK_Y</option>
<option value = 'VK_Z'>VK_Z</option>
<option value = 'VK_OPEN_BRACKET'>VK_OPEN_BRACKET</option>
<option value = 'VK_BACK_SLASH'>VK_BACK_SLASH</option>
<option value = 'VK_CLOSE_BRACKET'>VK_CLOSE_BRACKET</option>
<option value = 'VK_NUMPAD0'>VK_NUMPAD0</option>
<option value = 'VK_NUMPAD1'>VK_NUMPAD1</option>
<option value = 'VK_NUMPAD2'>VK_NUMPAD2</option>
<option value = 'VK_NUMPAD3'>VK_NUMPAD3</option>
<option value = 'VK_NUMPAD4'>VK_NUMPAD4</option>
<option value = 'VK_NUMPAD5'>VK_NUMPAD5</option>
<option value = 'VK_NUMPAD6'>VK_NUMPAD6</option>
<option value = 'VK_NUMPAD7'>VK_NUMPAD7</option>
<option value = 'VK_NUMPAD8'>VK_NUMPAD8</option>
<option value = 'VK_NUMPAD9'>VK_NUMPAD9</option>
<option value = 'VK_MULTIPLY'>VK_MULTIPLY</option>
<option value = 'VK_ADD'>VK_ADD</option>
<option value = 'VK_SEPARATER'>VK_SEPARATER</option>
<option value = 'VK_SUBTRACT'>VK_SUBTRACT</option>
<option value = 'VK_DECIMAL'>VK_DECIMAL</option>
<option value = 'VK_DIVIDE'>VK_DIVIDE</option>
<option value = 'VK_DELETE'>VK_DELETE</option>
<option value = 'VK_NUM_LOCK'>VK_NUM_LOCK</option>
<option value = 'VK_SCROLL_LOCK'>VK_SCROLL_LOCK</option>
<option value = 'VK_F1'>VK_F1</option>
<option value = 'VK_F2'>VK_F2</option>
<option value = 'VK_F3'>VK_F3</option>
<option value = 'VK_F4'>VK_F4</option>
<option value = 'VK_F5'>VK_F5</option>
<option value = 'VK_F6'>VK_F6</option>
<option value = 'VK_F7'>VK_F7</option>
<option value = 'VK_F8'>VK_F8</option>
<option value = 'VK_F9'>VK_F9</option>
<option value = 'VK_F10'>VK_F10</option>
<option value = 'VK_F11'>VK_F11</option>
<option value = 'VK_F12'>VK_F12</option>
<option value = 'VK_PRINTSCREEN'>VK_PRINTSCREEN</option>
<option value = 'VK_INSERT'>VK_INSERT</option>
<option value = 'VK_HELP'>VK_HELP</option>
<option value = 'VK_META'>VK_META</option>
<option value = 'VK_BACK_QUOTE'>VK_BACK_QUOTE</option>
<option value = 'VK_QUOTE'>VK_QUOTE</option>
<option value = 'VK_KP_UP'>VK_KP_UP</option>
<option value = 'VK_KP_DOWN'>VK_KP_DOWN</option>
<option value = 'VK_KP_LEFT'>VK_KP_LEFT</option>
<option value = 'VK_KP_RIGHT'>VK_KP_RIGHT</option>
</select>
<input type = submit>
<input type = button onclick='past()' value='control+V'>
<script>
	function past()
	{
		formKey.key1.value="VK_CONTROL";
		formKey.key2.value="VK_V";
		formKey.submit();
	}
</script>
</form>

<a href = '/jsp/control.jsp'>refresh</a>
<a href = '/'>back</a>
<form action = '/setQuality' method = 'post'>
quality:<input type = text name = 'quality' value = "<%=session.get("quality")%>">
<input type = submit>
</form>
<br>
<form action = '/ClipBoardAction' method = 'post'>
clipboard:<input type = text name = 'val'>
<input type = submit>
</form>
</body>
</html>
