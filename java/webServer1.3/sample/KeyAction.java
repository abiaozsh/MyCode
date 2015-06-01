package sample;

import java.awt.Robot;
import java.awt.event.KeyEvent;
import java.util.HashMap;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;
import webServer.Session;

@SuppressWarnings("boxing")
public class KeyAction extends Servlet
{
	public static HashMap<String, Integer> hm = new HashMap<String, Integer>();
	static
	{
		hm.put("VK_SHIFT", KeyEvent.VK_SHIFT);
		hm.put("VK_CONTROL", KeyEvent.VK_CONTROL);
		hm.put("VK_ALT", KeyEvent.VK_ALT);
		hm.put("VK_WINDOWS", KeyEvent.VK_WINDOWS);
		hm.put("VK_CONTEXT_MENU", KeyEvent.VK_CONTEXT_MENU);
		hm.put("VK_ENTER", KeyEvent.VK_ENTER);
		hm.put("VK_BACK_SPACE", KeyEvent.VK_BACK_SPACE);
		hm.put("VK_TAB", KeyEvent.VK_TAB);
		hm.put("VK_PAUSE", KeyEvent.VK_PAUSE);
		hm.put("VK_CAPS_LOCK", KeyEvent.VK_CAPS_LOCK);
		hm.put("VK_ESCAPE", KeyEvent.VK_ESCAPE);
		hm.put("VK_SPACE", KeyEvent.VK_SPACE);
		hm.put("VK_PAGE_UP", KeyEvent.VK_PAGE_UP);
		hm.put("VK_PAGE_DOWN", KeyEvent.VK_PAGE_DOWN);
		hm.put("VK_END", KeyEvent.VK_END);
		hm.put("VK_HOME", KeyEvent.VK_HOME);
		hm.put("VK_LEFT", KeyEvent.VK_LEFT);
		hm.put("VK_UP", KeyEvent.VK_UP);
		hm.put("VK_RIGHT", KeyEvent.VK_RIGHT);
		hm.put("VK_DOWN", KeyEvent.VK_DOWN);
		hm.put("VK_COMMA", KeyEvent.VK_COMMA);
		hm.put("VK_MINUS", KeyEvent.VK_MINUS);
		hm.put("VK_PERIOD", KeyEvent.VK_PERIOD);
		hm.put("VK_SLASH", KeyEvent.VK_SLASH);
		hm.put("VK_0", KeyEvent.VK_0);
		hm.put("VK_1", KeyEvent.VK_1);
		hm.put("VK_2", KeyEvent.VK_2);
		hm.put("VK_3", KeyEvent.VK_3);
		hm.put("VK_4", KeyEvent.VK_4);
		hm.put("VK_5", KeyEvent.VK_5);
		hm.put("VK_6", KeyEvent.VK_6);
		hm.put("VK_7", KeyEvent.VK_7);
		hm.put("VK_8", KeyEvent.VK_8);
		hm.put("VK_9", KeyEvent.VK_9);
		hm.put("VK_SEMICOLON", KeyEvent.VK_SEMICOLON);
		hm.put("VK_EQUALS", KeyEvent.VK_EQUALS);
		hm.put("VK_A", KeyEvent.VK_A);
		hm.put("VK_B", KeyEvent.VK_B);
		hm.put("VK_C", KeyEvent.VK_C);
		hm.put("VK_D", KeyEvent.VK_D);
		hm.put("VK_E", KeyEvent.VK_E);
		hm.put("VK_F", KeyEvent.VK_F);
		hm.put("VK_G", KeyEvent.VK_G);
		hm.put("VK_H", KeyEvent.VK_H);
		hm.put("VK_I", KeyEvent.VK_I);
		hm.put("VK_J", KeyEvent.VK_J);
		hm.put("VK_K", KeyEvent.VK_K);
		hm.put("VK_L", KeyEvent.VK_L);
		hm.put("VK_M", KeyEvent.VK_M);
		hm.put("VK_N", KeyEvent.VK_N);
		hm.put("VK_O", KeyEvent.VK_O);
		hm.put("VK_P", KeyEvent.VK_P);
		hm.put("VK_Q", KeyEvent.VK_Q);
		hm.put("VK_R", KeyEvent.VK_R);
		hm.put("VK_S", KeyEvent.VK_S);
		hm.put("VK_T", KeyEvent.VK_T);
		hm.put("VK_U", KeyEvent.VK_U);
		hm.put("VK_V", KeyEvent.VK_V);
		hm.put("VK_W", KeyEvent.VK_W);
		hm.put("VK_X", KeyEvent.VK_X);
		hm.put("VK_Y", KeyEvent.VK_Y);
		hm.put("VK_Z", KeyEvent.VK_Z);
		hm.put("VK_OPEN_BRACKET", KeyEvent.VK_OPEN_BRACKET);
		hm.put("VK_BACK_SLASH", KeyEvent.VK_BACK_SLASH);
		hm.put("VK_CLOSE_BRACKET", KeyEvent.VK_CLOSE_BRACKET);
		hm.put("VK_NUMPAD0", KeyEvent.VK_NUMPAD0);
		hm.put("VK_NUMPAD1", KeyEvent.VK_NUMPAD1);
		hm.put("VK_NUMPAD2", KeyEvent.VK_NUMPAD2);
		hm.put("VK_NUMPAD3", KeyEvent.VK_NUMPAD3);
		hm.put("VK_NUMPAD4", KeyEvent.VK_NUMPAD4);
		hm.put("VK_NUMPAD5", KeyEvent.VK_NUMPAD5);
		hm.put("VK_NUMPAD6", KeyEvent.VK_NUMPAD6);
		hm.put("VK_NUMPAD7", KeyEvent.VK_NUMPAD7);
		hm.put("VK_NUMPAD8", KeyEvent.VK_NUMPAD8);
		hm.put("VK_NUMPAD9", KeyEvent.VK_NUMPAD9);
		hm.put("VK_MULTIPLY", KeyEvent.VK_MULTIPLY);
		hm.put("VK_ADD", KeyEvent.VK_ADD);
		hm.put("VK_SEPARATER", KeyEvent.VK_SEPARATER);
		hm.put("VK_SUBTRACT", KeyEvent.VK_SUBTRACT);
		hm.put("VK_DECIMAL", KeyEvent.VK_DECIMAL);
		hm.put("VK_DIVIDE", KeyEvent.VK_DIVIDE);
		hm.put("VK_DELETE", KeyEvent.VK_DELETE);
		hm.put("VK_NUM_LOCK", KeyEvent.VK_NUM_LOCK);
		hm.put("VK_SCROLL_LOCK", KeyEvent.VK_SCROLL_LOCK);
		hm.put("VK_F1", KeyEvent.VK_F1);
		hm.put("VK_F2", KeyEvent.VK_F2);
		hm.put("VK_F3", KeyEvent.VK_F3);
		hm.put("VK_F4", KeyEvent.VK_F4);
		hm.put("VK_F5", KeyEvent.VK_F5);
		hm.put("VK_F6", KeyEvent.VK_F6);
		hm.put("VK_F7", KeyEvent.VK_F7);
		hm.put("VK_F8", KeyEvent.VK_F8);
		hm.put("VK_F9", KeyEvent.VK_F9);
		hm.put("VK_F10", KeyEvent.VK_F10);
		hm.put("VK_F11", KeyEvent.VK_F11);
		hm.put("VK_F12", KeyEvent.VK_F12);
		hm.put("VK_PRINTSCREEN", KeyEvent.VK_PRINTSCREEN);
		hm.put("VK_INSERT", KeyEvent.VK_INSERT);
		hm.put("VK_HELP", KeyEvent.VK_HELP);
		hm.put("VK_META", KeyEvent.VK_META);
		hm.put("VK_BACK_QUOTE", KeyEvent.VK_BACK_QUOTE);
		hm.put("VK_QUOTE", KeyEvent.VK_QUOTE);
		hm.put("VK_KP_UP", KeyEvent.VK_KP_UP);
		hm.put("VK_KP_DOWN", KeyEvent.VK_KP_DOWN);
		hm.put("VK_KP_LEFT", KeyEvent.VK_KP_LEFT);
		hm.put("VK_KP_RIGHT", KeyEvent.VK_KP_RIGHT);
	}

	@Override
	public void doReq(Request req, Response res, Server server) throws Exception
	{
		String skey1 = req.getPOSTParam("key1");
		String skey2 = req.getPOSTParam("key2");

		try
		{
			Robot r = new Robot();

			Integer key1 = hm.get(skey1);
			Integer key2 = hm.get(skey2);

			if (key2 == null && key1.equals(KeyEvent.VK_WINDOWS))
			{
				r.keyPress(KeyEvent.VK_WINDOWS);
				r.keyRelease(KeyEvent.VK_WINDOWS);
			}

			if (key1 == null)
			{
				r.keyPress(key2.intValue());
				r.keyRelease(key2.intValue());
			}

			if (key1 != null && key2 != null)
			{
				r.keyPress(key1.intValue());
				r.keyPress(key2.intValue());
				r.keyRelease(key1.intValue());
				r.keyRelease(key2.intValue());
			}
		}
		catch (Exception e)
		{
			//
		}
		Session s = req.getSession();
		if (s.get("Redirect") != null)
		{
			res.sendRedirect((String) s.get("Redirect"));
		}
		else
		{
			res.sendRedirect("/jsp/control.jsp");
		}
	}
}
