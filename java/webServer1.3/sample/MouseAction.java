package sample;

import java.awt.Robot;
import java.awt.event.InputEvent;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;
import webServer.Session;

public class MouseAction extends Servlet
{
	@Override
	public void doReq(Request req, Response res, Server server) throws Exception
	{
		String action = req.getPOSTParam("action");
		int x = strToInt(req.getPOSTParam("x"));
		int y = strToInt(req.getPOSTParam("y"));
		if ("1".equals(action))
		{
			Robot robot = null;
			try
			{
				robot = new Robot();
				robot.mouseMove(x, y);
				robot.mousePress(InputEvent.BUTTON1_MASK);
				robot.mouseRelease(InputEvent.BUTTON1_MASK);
			}
			catch (Exception e)
			{
				System.out.println(e.toString());
			}
		}
		if ("2".equals(action))
		{
			Robot robot = null;
			try
			{
				robot = new Robot();
				robot.mouseMove(x, y);
				robot.mousePress(InputEvent.BUTTON1_MASK);
				robot.mouseRelease(InputEvent.BUTTON1_MASK);
				robot.mousePress(InputEvent.BUTTON1_MASK);
				robot.mouseRelease(InputEvent.BUTTON1_MASK);
			}
			catch (Exception e)
			{
				System.out.println(e.toString());
			}
		}
		if ("3".equals(action))
		{
			Robot robot = null;
			try
			{
				robot = new Robot();
				robot.mouseMove(x, y);
				robot.mousePress(InputEvent.BUTTON3_MASK);
				robot.mouseRelease(InputEvent.BUTTON3_MASK);
			}
			catch (Exception e)
			{
				System.out.println(e.toString());
			}
		}
		if ("4".equals(action))
		{
			Robot robot = null;
			try
			{
				robot = new Robot();
				robot.mouseMove(x, y);
				robot.mousePress(InputEvent.BUTTON1_MASK);
			}
			catch (Exception e)
			{
				System.out.println(e.toString());
			}
		}
		if ("5".equals(action))
		{
			Robot robot = null;
			try
			{
				robot = new Robot();
				robot.mouseMove(x, y);
				robot.mouseRelease(InputEvent.BUTTON1_MASK);
			}
			catch (Exception e)
			{
				System.out.println(e.toString());
			}
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

	private int strToInt(String sNum)
	{
		int iNum = -1;
		if (sNum != null && sNum.length() >= 0)
		{
			try
			{
				iNum = Integer.parseInt(sNum);
			}
			catch (NumberFormatException e)
			{
				iNum = -1;
			}
		}
		return iNum;
	}
}
