package webServer;

import java.io.PrintWriter;
import java.util.Stack;

public abstract class JspPage
{
    String url;

    public PrintWriter out;

    public Request request;

    public Response response;

    public Session session;

    public Server server;

    long lastModified;

    Stack<Class<? extends JspPage>> callingStack;

    public abstract boolean authorization(String username, String password);

    private final void doReq(Request req, Response res, PrintWriter out, Server server, Stack<Class<? extends JspPage>> callingStack) throws Exception
    {
        this.callingStack = callingStack;
        this.out = out;
        this.request = req;
        this.response = res;
        this.session = req.getSession();
        this.server = server;
        doRequest();
    }

    final void doReq(Request req, Response res, Server server) throws Exception
    {
        callingStack = new Stack<Class<? extends JspPage>>();
        this.out = res.getWriter();
        this.request = req;
        this.response = res;
        this.session = req.getSession();
        this.server = server;
        doRequest();
        out.flush();
    }

    public abstract void doRequest() throws Exception;

    protected void includeJSP(String jsp) throws Exception
    {
        String[] level3 = jsp.split("\\?");// ?
        String filename = level3[0];
        if (level3.length > 1)
        {
            String[] level4 = level3[1].split("&");
            for (int j = 0; j < level4.length; j++)
            {
                String[] level5 = level4[j].split("=");
                if (level5.length == 2)
                {
                    request.setGETParam(level5[0], level5[1]);
                }
            }
        }

        if (!filename.startsWith("/"))
        {
            filename = request.getPath().substring(0, request.getPath().lastIndexOf("/")) + "/" + filename;
        }

        synchronized(server.jspProcessors){
            for (JspProcessor jp : server.jspProcessors)
            {
                if (filename.startsWith(jp.getUrl()))
                {
                    JspPage jspPage = jp.getNewJspPage(filename, server);
                    if (jspPage != null)
                    {
                        if (callingStack.search(this.getClass()) != -1)
                        {
                            throw new Exception("jsp include is dead locked(caused by a looping include).");
                        }
                        callingStack.push(this.getClass());
                        jspPage.doReq(request, response, out, server, callingStack);
                        callingStack.pop();
                        return;
                    }
                }
            }
        }
        throw new Exception("jsp include \"" + filename + "\" not found.");
    }
}