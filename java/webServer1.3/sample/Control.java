package sample;

import java.io.PrintWriter;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;
import webServer.Session;

public class Control extends Servlet
{

	@Override
	public boolean authorization(String username, String password, Server server)
	{
		if ("zhengsh".equals(username) && "zhengsh".equals(password))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	@Override
	public void doReq(Request req, Response res, Server server) throws Exception
	{
		Session session = req.getSession();

		session.set("Redirect", "Control");
		session.set("pass", "ok");

		PrintWriter out = res.getWriter();

		out.println("<html>");
		out.println("<head>");
		out.println("<title>title</title></head>");
		out.println("<body onload = 'document.onkeydown = keyDown;'>");
		out.println("<script type=\"text/javascript\">");
		out.println("var posx;var posy;");
		out.println("function getMouse(e){");
		out.println("	posx=0;posy=0;");
		out.println("	var ev=(!e)?window.event:e;//IE:Moz");
		out.println("	if (ev.pageX){//Moz");
		out.println("		posx=ev.pageX;//+window.pageXOffset;");
		out.println("		posy=ev.pageY;//+window.pageYOffset;");
		out.println("		document.formMain.x.value = posx-8;");
		out.println("		document.formMain.y.value = posy-8;");
		out.println("	}");
		out.println("	else if(ev.clientX){//IE");
		out.println("		posx=ev.clientX+document.body.scrollLeft;");
		out.println("		posy=ev.clientY+document.body.scrollTop;");
		out.println("		document.formMain.x.value = posx-12;");
		out.println("		document.formMain.y.value = posy-17;");
		out.println("	}");
		out.println("	else{return false}//old browsers");
		out.println("}");
		out.println("document.onmousemove=getMouse");
		out.println("</script>");
		out.println("<script>");
		out.println("function keyDown()");
		out.println("{");
		out.println(" var k = event.keyCode;");
		out.println(" if(k==17)formMain.action.value = 2;");
		out.println(" else if(k==16)formMain.action.value = 3;");
		out.println("}");
		out.println("function mouseDown(){");
		out.println(" document.formMain.submit();");
		out.println("}");
		out.println("</script>");
		out.println("<img src='/Screen' onclick = 'mouseDown()'>");
		out.println("<form name = 'formMain' action = '/MouseAction' method = 'post'>");
		out.println("<select name = 'action'>");
		out.println("<option value = '1'>single click</option>");
		out.println("<option value = '2'>double click</option>");
		out.println("<option value = '3'>right click</option>");
		out.println("<option value = '4'>drag</option>");
		out.println("<option value = '5'>drop</option>");
		out.println("</select>");
		out.println("<input type = text name = 'x'>");
		out.println("<input type = text name = 'y'>");
		out.println("</form>");
		out.println("<form action = '/KeyAction' method = 'post'>");
		out.println("<select name = 'key1'>");
		out.println("<option value = ''>NONE</option>");
		out.println("<option value = 'VK_SHIFT'>VK_SHIFT</option>");
		out.println("<option value = 'VK_CONTROL'>VK_CONTROL</option>");
		out.println("<option value = 'VK_ALT'>VK_ALT</option>");
		out.println("<option value = 'VK_WINDOWS'>VK_WINDOWS</option>");
		out.println("</select>");
		out.println("<select name = 'key2'>");
		out.println("<option value = ''>NONE</option>");
		out.println("<option value = 'VK_WINDOWS'>VK_WINDOWS</option>");
		out.println("<option value = 'VK_CONTEXT_MENU'>VK_CONTEXT_MENU</option>");
		out.println("<option value = 'VK_ENTER'>VK_ENTER</option>");
		out.println("<option value = 'VK_BACK_SPACE'>VK_BACK_SPACE</option>");
		out.println("<option value = 'VK_TAB'>VK_TAB</option>");
		out.println("<option value = 'VK_PAUSE'>VK_PAUSE</option>");
		out.println("<option value = 'VK_CAPS_LOCK'>VK_CAPS_LOCK</option>");
		out.println("<option value = 'VK_ESCAPE'>VK_ESCAPE</option>");
		out.println("<option value = 'VK_SPACE'>VK_SPACE</option>");
		out.println("<option value = 'VK_PAGE_UP'>VK_PAGE_UP</option>");
		out.println("<option value = 'VK_PAGE_DOWN'>VK_PAGE_DOWN</option>");
		out.println("<option value = 'VK_END'>VK_END</option>");
		out.println("<option value = 'VK_HOME'>VK_HOME</option>");
		out.println("<option value = 'VK_LEFT'>VK_LEFT</option>");
		out.println("<option value = 'VK_UP'>VK_UP</option>");
		out.println("<option value = 'VK_RIGHT'>VK_RIGHT</option>");
		out.println("<option value = 'VK_DOWN'>VK_DOWN</option>");
		out.println("<option value = 'VK_COMMA'>VK_COMMA</option>");
		out.println("<option value = 'VK_MINUS'>VK_MINUS</option>");
		out.println("<option value = 'VK_PERIOD'>VK_PERIOD</option>");
		out.println("<option value = 'VK_SLASH'>VK_SLASH</option>");
		out.println("<option value = 'VK_0'>VK_0</option>");
		out.println("<option value = 'VK_1'>VK_1</option>");
		out.println("<option value = 'VK_2'>VK_2</option>");
		out.println("<option value = 'VK_3'>VK_3</option>");
		out.println("<option value = 'VK_4'>VK_4</option>");
		out.println("<option value = 'VK_5'>VK_5</option>");
		out.println("<option value = 'VK_6'>VK_6</option>");
		out.println("<option value = 'VK_7'>VK_7</option>");
		out.println("<option value = 'VK_8'>VK_8</option>");
		out.println("<option value = 'VK_9'>VK_9</option>");
		out.println("<option value = 'VK_SEMICOLON'>VK_SEMICOLON</option>");
		out.println("<option value = 'VK_EQUALS'>VK_EQUALS</option>");
		out.println("<option value = 'VK_A'>VK_A</option>");
		out.println("<option value = 'VK_B'>VK_B</option>");
		out.println("<option value = 'VK_C'>VK_C</option>");
		out.println("<option value = 'VK_D'>VK_D</option>");
		out.println("<option value = 'VK_E'>VK_E</option>");
		out.println("<option value = 'VK_F'>VK_F</option>");
		out.println("<option value = 'VK_G'>VK_G</option>");
		out.println("<option value = 'VK_H'>VK_H</option>");
		out.println("<option value = 'VK_I'>VK_I</option>");
		out.println("<option value = 'VK_J'>VK_J</option>");
		out.println("<option value = 'VK_K'>VK_K</option>");
		out.println("<option value = 'VK_L'>VK_L</option>");
		out.println("<option value = 'VK_M'>VK_M</option>");
		out.println("<option value = 'VK_N'>VK_N</option>");
		out.println("<option value = 'VK_O'>VK_O</option>");
		out.println("<option value = 'VK_P'>VK_P</option>");
		out.println("<option value = 'VK_Q'>VK_Q</option>");
		out.println("<option value = 'VK_R'>VK_R</option>");
		out.println("<option value = 'VK_S'>VK_S</option>");
		out.println("<option value = 'VK_T'>VK_T</option>");
		out.println("<option value = 'VK_U'>VK_U</option>");
		out.println("<option value = 'VK_V'>VK_V</option>");
		out.println("<option value = 'VK_W'>VK_W</option>");
		out.println("<option value = 'VK_X'>VK_X</option>");
		out.println("<option value = 'VK_Y'>VK_Y</option>");
		out.println("<option value = 'VK_Z'>VK_Z</option>");
		out.println("<option value = 'VK_OPEN_BRACKET'>VK_OPEN_BRACKET</option>");
		out.println("<option value = 'VK_BACK_SLASH'>VK_BACK_SLASH</option>");
		out.println("<option value = 'VK_CLOSE_BRACKET'>VK_CLOSE_BRACKET</option>");
		out.println("<option value = 'VK_NUMPAD0'>VK_NUMPAD0</option>");
		out.println("<option value = 'VK_NUMPAD1'>VK_NUMPAD1</option>");
		out.println("<option value = 'VK_NUMPAD2'>VK_NUMPAD2</option>");
		out.println("<option value = 'VK_NUMPAD3'>VK_NUMPAD3</option>");
		out.println("<option value = 'VK_NUMPAD4'>VK_NUMPAD4</option>");
		out.println("<option value = 'VK_NUMPAD5'>VK_NUMPAD5</option>");
		out.println("<option value = 'VK_NUMPAD6'>VK_NUMPAD6</option>");
		out.println("<option value = 'VK_NUMPAD7'>VK_NUMPAD7</option>");
		out.println("<option value = 'VK_NUMPAD8'>VK_NUMPAD8</option>");
		out.println("<option value = 'VK_NUMPAD9'>VK_NUMPAD9</option>");
		out.println("<option value = 'VK_MULTIPLY'>VK_MULTIPLY</option>");
		out.println("<option value = 'VK_ADD'>VK_ADD</option>");
		out.println("<option value = 'VK_SEPARATER'>VK_SEPARATER</option>");
		out.println("<option value = 'VK_SUBTRACT'>VK_SUBTRACT</option>");
		out.println("<option value = 'VK_DECIMAL'>VK_DECIMAL</option>");
		out.println("<option value = 'VK_DIVIDE'>VK_DIVIDE</option>");
		out.println("<option value = 'VK_DELETE'>VK_DELETE</option>");
		out.println("<option value = 'VK_NUM_LOCK'>VK_NUM_LOCK</option>");
		out.println("<option value = 'VK_SCROLL_LOCK'>VK_SCROLL_LOCK</option>");
		out.println("<option value = 'VK_F1'>VK_F1</option>");
		out.println("<option value = 'VK_F2'>VK_F2</option>");
		out.println("<option value = 'VK_F3'>VK_F3</option>");
		out.println("<option value = 'VK_F4'>VK_F4</option>");
		out.println("<option value = 'VK_F5'>VK_F5</option>");
		out.println("<option value = 'VK_F6'>VK_F6</option>");
		out.println("<option value = 'VK_F7'>VK_F7</option>");
		out.println("<option value = 'VK_F8'>VK_F8</option>");
		out.println("<option value = 'VK_F9'>VK_F9</option>");
		out.println("<option value = 'VK_F10'>VK_F10</option>");
		out.println("<option value = 'VK_F11'>VK_F11</option>");
		out.println("<option value = 'VK_F12'>VK_F12</option>");
		out.println("<option value = 'VK_PRINTSCREEN'>VK_PRINTSCREEN</option>");
		out.println("<option value = 'VK_INSERT'>VK_INSERT</option>");
		out.println("<option value = 'VK_HELP'>VK_HELP</option>");
		out.println("<option value = 'VK_META'>VK_META</option>");
		out.println("<option value = 'VK_BACK_QUOTE'>VK_BACK_QUOTE</option>");
		out.println("<option value = 'VK_QUOTE'>VK_QUOTE</option>");
		out.println("<option value = 'VK_KP_UP'>VK_KP_UP</option>");
		out.println("<option value = 'VK_KP_DOWN'>VK_KP_DOWN</option>");
		out.println("<option value = 'VK_KP_LEFT'>VK_KP_LEFT</option>");
		out.println("<option value = 'VK_KP_RIGHT'>VK_KP_RIGHT</option>");
		out.println("</select>");
		out.println("<input type = submit>");
		out.println("</form>");
		out.println("<a href = '/jsp/control.jsp'>refresh</a>");
		out.println("<a href = '/'>back</a>");
		out.println("<form action = '/setQuality' method = 'post'>");
		out.print("quality:<input type = text name = 'quality' value = \"");
		out.print(session.get("quality"));
		out.println("\">");
		out.println("<input type = submit>");
		out.println("</form>");
		out.println("<br>");
		out.println("<form action = '/ClipBoardAction' method = 'post'>");
		out.println("clipboard:<input type = text name = 'val'>");
		out.println("<input type = submit>");
		out.println("</form>");
		out.println("</body>");
		out.println("</html>");
	}
}
