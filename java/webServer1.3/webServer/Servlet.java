package webServer;

public abstract class Servlet
{

	String url;

	public abstract void doReq(Request req, Response res, Server server) throws Exception;

	public boolean authorization(String username, String password, Server server)
	{
		return true;
	}

	public void dispatch(Request req, Response res, Server server, String target)
	{
		server.getRequestDispatcher(target).forward(req, res);
	}
}
