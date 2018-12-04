package sample;

import java.io.File;
import java.io.FileOutputStream;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;
import webServer.Session;

public class UploadServlet extends Servlet
{
	@Override
	public void doReq(Request req, Response res, Server server) throws Exception
	{
		Session session = req.getSession();
		if (!"T".equals(session.get(LoginServlet.LOGIN))) {
			res.sendRedirect("Login");
			return;
		}

		if (req.getMultipartData("file1") != null)
		{
			File file = new File(req.getMultipartData("file1").filename);
			try
			{
				FileOutputStream s = new FileOutputStream("fileSystem\\" + file.getName());
				s.write(req.getMultipartData("file1").bArray);
				s.close();
			}
			catch (Exception e)
			{
				System.out.print(e);
			}
		}
		if (req.getMultipartData("file2") != null)
		{
			File file = new File(req.getMultipartData("file2").filename);
			try
			{
				FileOutputStream s = new FileOutputStream("fileSystem\\" + file.getName());
				s.write(req.getMultipartData("file2").bArray);
				s.close();
			}
			catch (Exception e)
			{
				System.out.print(e);
			}
		}
		res.sendRedirect("/file/");
	}
}
