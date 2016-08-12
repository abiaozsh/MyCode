package sample;

import java.io.PrintWriter;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;
import webServer.Session;

public class ControlV2 extends Servlet {

	@Override
	public void doReq(Request req, Response res, Server server) throws Exception {
		Session session = req.getSession();

		if (!"T".equals(session.get(LoginServlet.LOGIN))) {
			res.sendRedirect("Login");
			return;
		}

		session.set("Redirect", "ControlV2");

		PrintWriter out = res.getWriter();

		out.println("<html>");
		out.println("<head>");
		out.println("<title>title</title>");
		out.println("<script type='text/javascript' src='js/jquery-3.1.0.js'></script>");
		out.println("<script type='text/javascript' src='js/Control.js'></script>");
		
		out.println("<style type='text/css'>");
		out.println(".btnon {background-color:#FDD;}");
		out.println(".btnoff {background-color:#DDD;}");
		out.println("</style>");

		out.println("</head>");
		out.println("<body>");
		
		out.println("<input type='button' class='J_MouseButton btnon' data-val='0' value='auto'/>");
		out.println("<input type='button' class='J_MouseButton btnoff' data-val='1' value='single click'/>");
		out.println("<input type='button' class='J_MouseButton btnoff' data-val='2' value='double click'/>");
		out.println("<input type='button' class='J_MouseButton btnoff' data-val='3' value='right click'/>");
		out.println("<input type='button' class='J_MouseButton btnoff' data-val='4' value='drag'/>");
		out.println("<input type='button' class='J_MouseButton btnoff' data-val='5' value='drop'/>");
		out.println("<input type='button' id='Refresh' value='Refresh'/>");
		out.println("<input type='button' id='J_Copykey' value='ctrl+C'>");
		out.println("<input type='button' id='J_Cutkey' value='ctrl+X'>");
		out.println("<input type='button' id='J_Pastkey' value='ctrl+V'><br/>");

		out.println("<img src='Screen.jpg' id='MainImg'><br/>");

		out.println("<table>");
		out.println("<tr>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_ESCAPE' value='ESC'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F1' value='F1'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F2' value='F2'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F3' value='F3'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F4' value='F4'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F5' value='F5'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F6' value='F6'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F7' value='F7'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F8' value='F8'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F9' value='F9'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F10' value='F10'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F11' value='F11'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F12' value='F12'/></td>");
		out.println("</tr>");
		
		out.println("<tr>");
		out.println("	<td></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_1' value='1'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_2' value='2'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_3' value='3'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_4' value='4'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_5' value='5'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_6' value='6'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_7' value='7'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_8' value='8'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_9' value='9'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_0' value='0'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_BACK_SPACE' value='BACK_SPACE'/></td>");
		out.println("</tr>");

		out.println("<tr>");
		out.println("	<td></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_Q' value='Q'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_W' value='W'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_E' value='E'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_R' value='R'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_T' value='T'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_Y' value='Y'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_U' value='U'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_I' value='I'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_O' value='O'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_P' value='P'/></td>");
		out.println("	<td></td>");
		out.println("	<td></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_UP' value='UP'/></td>");
		out.println("</tr>");

		out.println("<tr>");
		out.println("	<td></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_A' value='A'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_S' value='S'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_D' value='D'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_F' value='F'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_G' value='G'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_H' value='H'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_J' value='J'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_K' value='K'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_L' value='L'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_ENTER' value='ENTER'/></td>");
		out.println("	<td></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_LEFT' value='LEFT'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_DOWN' value='DOWN'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_RIGHT' value='RIGHT'/></td>");
		out.println("</tr>");

		out.println("<tr>");
		out.println("	<td></td>");
		out.println("	<td></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_Z' value='Z'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_X' value='X'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_C' value='C'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_V' value='V'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_B' value='B'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_N' value='N'/></td>");
		out.println("	<td><input type='button' class='J_KeyButton' data-val='VK_M' value='M'/></td>");
		out.println("</tr>");

		out.println("</table>");
		
		out.println("<select id = 'key1'>");
		out.println("<option value = ''>NONE</option>");
		out.println("<option value = 'VK_SHIFT'>VK_SHIFT</option>");
		out.println("<option value = 'VK_CONTROL'>VK_CONTROL</option>");
		out.println("<option value = 'VK_ALT'>VK_ALT</option>");
		out.println("<option value = 'VK_WINDOWS'>VK_WINDOWS</option>");
		out.println("</select>");
		out.println("<select id = 'key2'>");
		out.println("<option value = ''>NONE</option>");
		out.println("<option value = 'VK_WINDOWS'>VK_WINDOWS</option>");
		out.println("<option value = 'VK_TAB'>VK_TAB</option>");
		out.println("<option value = 'VK_SPACE'>VK_SPACE</option>");
		out.println("<option value = 'VK_CONTEXT_MENU'>VK_CONTEXT_MENU</option>");
		out.println("<option value = 'VK_PAGE_UP'>VK_PAGE_UP</option>");
		out.println("<option value = 'VK_PAGE_DOWN'>VK_PAGE_DOWN</option>");
		out.println("<option value = 'VK_END'>VK_END</option>");
		out.println("<option value = 'VK_HOME'>VK_HOME</option>");
		out.println("<option value = 'VK_COMMA'>VK_COMMA</option>");
		out.println("<option value = 'VK_MINUS'>VK_MINUS</option>");
		out.println("<option value = 'VK_PERIOD'>VK_PERIOD</option>");
		out.println("<option value = 'VK_SLASH'>VK_SLASH</option>");
		out.println("<option value = 'VK_SEMICOLON'>VK_SEMICOLON</option>");
		out.println("<option value = 'VK_EQUALS'>VK_EQUALS</option>");
		out.println("<option value = 'VK_OPEN_BRACKET'>VK_OPEN_BRACKET</option>");
		out.println("<option value = 'VK_BACK_SLASH'>VK_BACK_SLASH</option>");
		out.println("<option value = 'VK_CLOSE_BRACKET'>VK_CLOSE_BRACKET</option>");
		out.println("<option value = 'VK_MULTIPLY'>VK_MULTIPLY</option>");
		out.println("<option value = 'VK_ADD'>VK_ADD</option>");
		out.println("<option value = 'VK_SEPARATER'>VK_SEPARATER</option>");
		out.println("<option value = 'VK_SUBTRACT'>VK_SUBTRACT</option>");
		out.println("<option value = 'VK_DECIMAL'>VK_DECIMAL</option>");
		out.println("<option value = 'VK_DIVIDE'>VK_DIVIDE</option>");
		out.println("<option value = 'VK_DELETE'>VK_DELETE</option>");
		out.println("<option value = 'VK_NUM_LOCK'>VK_NUM_LOCK</option>");
		out.println("<option value = 'VK_SCROLL_LOCK'>VK_SCROLL_LOCK</option>");
		out.println("<option value = 'VK_PRINTSCREEN'>VK_PRINTSCREEN</option>");
		out.println("<option value = 'VK_INSERT'>VK_INSERT</option>");
		out.println("<option value = 'VK_HELP'>VK_HELP</option>");
		out.println("<option value = 'VK_BACK_QUOTE'>VK_BACK_QUOTE</option>");
		out.println("<option value = 'VK_QUOTE'>VK_QUOTE</option>");
		out.println("<option value = 'VK_KP_UP'>VK_KP_UP</option>");
		out.println("<option value = 'VK_KP_DOWN'>VK_KP_DOWN</option>");
		out.println("<option value = 'VK_KP_LEFT'>VK_KP_LEFT</option>");
		out.println("<option value = 'VK_KP_RIGHT'>VK_KP_RIGHT</option>");
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
		out.println("<option value = 'VK_PAUSE'>VK_PAUSE</option>");
		out.println("<option value = 'VK_META'>VK_META</option>");
		out.println("<option value = 'VK_CAPS_LOCK'>VK_CAPS_LOCK</option>");
		out.println("</select>");
		out.println("<input type = 'button' value='Send' id='SendKey'>");

		out.println("<br/>");

		out.println("quality:<input type='text' id='J_Quality' value = '" + session.get("quality") + "'>");
		out.println("<input type = 'button' value='Send' id='SendQuality'>");

		out.println("<br/>");

		out.println("clipboard:<input type='text' id='J_ClipBoard'>");
		out.println("<input type = 'button' value='Send' id='SendClipboard'>");
		out.println("</form>");
		out.println("</body>");
		out.println("</html>");
	}
}
