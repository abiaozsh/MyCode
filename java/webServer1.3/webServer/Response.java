package webServer;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.zip.GZIPOutputStream;

public class Response
{
    public final static String SERVER_NAME = "mini WebServer/1.2";

    public final static String JSESSIONID = "JSESSIONID";

    public final static String HEAD200 = "HTTP/1.1 200 OK";

    public final static String HEAD206 = "HTTP/1.1 206 Partial content";

    public final static String HEAD302 = "HTTP/1.1 302 Moved Temporarily";

    public final static String HEAD401 = "HTTP/1.1 401 N/A";

    public final static String HEAD404 = "HTTP/1.1 404 Not Found";

    public final static String HEAD500 = "HTTP/1.1 500 Internal Server Error";

    Request req;

    String head;

    boolean closeConnection = false;

    HashMap<String, String> contents;

    HashMap<String, Object> attribute;

    ArrayList<Cookie> responseCookies;

    ByteArrayOutputStream byteArrayOutputStream;

    GZIPOutputStream gZIPOutputStream;

    int inputStreamLength = -1;

    boolean GZIP = false;

    InputStream inputStream;

    boolean inputStreamClose = true;

    PrintWriter writer;

    String characterEncoding;

    String contentType;

    String fileName;

    int status = 0;

    final static int STATUS_BINARY = 1;

    final static int STATUS_TEXT = 2;

    final static int STATUS_INPUT = 3;

    final static int STATUS_REDIRECT = 4;

    Server server;

    public void closeConnection()
    {
        closeConnection = true;
    }

    @Override
    protected void finalize() throws Throwable
    {
        try
        {
            if (inputStreamClose && inputStream != null)
            {
                inputStream.close();
            }
        }
        catch (Exception e)
        {
            // do nothing
        }
        super.finalize();
    }

    Response(Request req, Server server)
    {
        head = HEAD200;
        contents = new HashMap<String, String>();
        setHeader("Server", SERVER_NAME);

        attribute = new HashMap<String, Object>();
        responseCookies = new ArrayList<Cookie>();
        this.server = server;
    }

    public String getCharacterEncoding()
    {
        return characterEncoding;
    }

    public void setCharacterEncoding(String characterEncoding)
    {
        this.characterEncoding = characterEncoding;
    }

    public Object getAttribute(String name)
    {
        return attribute.get(name);
    }

    public void setAttribute(String name, Object obj)
    {
        attribute.put(name, obj);
    }

    public OutputStream getOutputStream() throws IOException
    {
        if (status == 0)
        {
            status = STATUS_BINARY;
            contentType = "application/octet-stream";
            if (GZIP)
            {
                byteArrayOutputStream = new ByteArrayOutputStream();
                gZIPOutputStream = new GZIPOutputStream(byteArrayOutputStream);
                return gZIPOutputStream;
            }
            else
            {
                byteArrayOutputStream = new ByteArrayOutputStream();
                return byteArrayOutputStream;
            }
        }
        else
        {
            throw new IllegalStateException();
        }
    }

    public PrintWriter getWriter() throws IOException
    {
        return getWriter(null);
    }

    public PrintWriter getWriter(String characterEncoding) throws IOException
    {
        if (status == 0)
        {
            status = STATUS_TEXT;
            contentType = "text/html";
            if (characterEncoding != null)
            {
                this.characterEncoding = characterEncoding;
            }
            else
            {
                this.characterEncoding = server.currentConfig.defaultEncoding;
            }
            if (GZIP)
            {
                byteArrayOutputStream = new ByteArrayOutputStream();
                gZIPOutputStream = new GZIPOutputStream(byteArrayOutputStream);
                OutputStreamWriter osw = new OutputStreamWriter(gZIPOutputStream, this.characterEncoding);
                writer = new PrintWriter(osw);
                return writer;
            }
            else
            {
                byteArrayOutputStream = new ByteArrayOutputStream();
                OutputStreamWriter osw = new OutputStreamWriter(byteArrayOutputStream, this.characterEncoding);
                writer = new PrintWriter(osw);
                return writer;
            }
        }
        else
        {
            throw new IllegalStateException();
        }
    }

    public void setInputStream(InputStream is)
    {
        setInputStream(is, -1, true);
    }

    public void setInputStream(InputStream is, int isLength)
    {
        setInputStream(is, isLength, true);
    }

    public void setInputStream(InputStream is, boolean inputStreamClose)
    {
        setInputStream(is, -1, inputStreamClose);
    }

    public void setInputStream(InputStream is, int isLength, boolean inputStreamClose)
    {
        if (status == 0)
        {
            contentType = "application/octet-stream";
            status = STATUS_INPUT;
            inputStream = is;
            this.inputStreamClose = inputStreamClose;
            inputStreamLength = isLength;
            GZIP = false;
        }
        else
        {
            throw new IllegalStateException();
        }
    }

    public String getFileName()
    {
        return fileName;
    }

    public void setFileName(String fileName)
    {
        this.fileName = fileName;
    }

    public void setHeader(String header)
    {
        this.head = header;
    }

    public void setHeader(String name, String value)
    {
        contents.put(name, value);
    }

    public String getContentType()
    {
        return contentType;
    }

    public void setContentType(String contentType)
    {
        this.contentType = contentType;
    }

    public void addCookie(String name, String value, String path)
    {
        Cookie cookie = new Cookie();
        cookie.name = name;
        cookie.value = value;
        cookie.maxAge = -1;
        cookie.path = path;
        responseCookies.add(cookie);
    }

    public void addCookie(String name, String value, int maxAge, String path)
    {
        Cookie cookie = new Cookie();
        cookie.name = name;
        cookie.value = value;
        cookie.maxAge = maxAge;
        cookie.path = path;
        responseCookies.add(cookie);
    }

    public void removeCookie(String name, String path)
    {
        Cookie cookie = new Cookie();
        cookie.name = name;
        cookie.value = "";
        cookie.maxAge = 0;
        cookie.path = path;
        responseCookies.add(cookie);
    }

    public void sendRedirect(String redirect)
    {
        if (status == 0)
        {
            status = STATUS_REDIRECT;
            head = HEAD302;
            this.setHeader("Pragma", "no-cache");
            this.setHeader("Cache-Control", "no-cache");
            this.setHeader("Expires", "-1");
            this.setHeader("Location", redirect);
        }
        else
        {
            throw new IllegalStateException();
        }
    }

    void notFound()
    {
        try
        {
            clear();
            head = HEAD404;
            GZIP = false;
            req.session = null;
            req.sessionIsNullFirstTime = false;
            PrintWriter out = getWriter();

            out.println("<HTML>");
            out.println("<HEAD>");
            out.println("<TITLE>" + SERVER_NAME + " - Error report</TITLE>");
            out.println("<META http-equiv=Content-Type content=\"text/html; charset=utf-8\">");
            out.println("<STYLE>");
            out.println("H1 {FONT-SIZE: 22px; COLOR: white; FONT-FAMILY: Tahoma,Arial,sans-serif; BACKGROUND-COLOR: #525d76}");
            out.println("H2 {FONT-SIZE: 16px; COLOR: white; FONT-FAMILY: Tahoma,Arial,sans-serif; BACKGROUND-COLOR: #525d76}");
            out.println("H3 {FONT-SIZE: 14px; COLOR: white; FONT-FAMILY: Tahoma,Arial,sans-serif; BACKGROUND-COLOR: #525d76}");
            out.println("BODY {COLOR: black; FONT-FAMILY: Tahoma,Arial,sans-serif; BACKGROUND-COLOR: white}");
            out.println("B {COLOR: white; FONT-FAMILY: Tahoma,Arial,sans-serif; BACKGROUND-COLOR: #525d76}");
            out.println("P {FONT-SIZE: 12px; BACKGROUND: white; COLOR: black; FONT-FAMILY: Tahoma,Arial,sans-serif}");
            out.println("A {COLOR: black}");
            out.println("A.name {COLOR: black}");
            out.println("HR {COLOR: #525d76}");
            out.println("</STYLE>");
            out.println("</HEAD>");
            out.println("<BODY>");
            out.println("<H1>HTTP Status 404 - " + req.getPath() + "</H1>");
            out.println("<HR noShade SIZE=1>");
            out.println("<P><B>type</B> Status report</P>");
            out.println("<P><B>message</B> <U>" + req.getPath() + "</U></P>");
            out.println("<P><B>description</B> <U>The requested resource (" + req.getPath() + ") is not available.</U></P>");
            out.println("<HR noShade SIZE=1>");
            out.println("<H3>" + SERVER_NAME + "</H3></BODY></HTML>");
            this.closeConnection();
        }
        catch (IOException ex)
        {
            //
        }
    }

    void exceptionPage(Throwable e)
    {
        try
        {
            clear();
            head = HEAD500;
            GZIP = false;
            req.session = null;
            req.sessionIsNullFirstTime = false;
            PrintWriter out = getWriter();
            out.println("<HTML>");
            out.println("<HEAD>");
            out.println("<TITLE>" + SERVER_NAME + " - Error report</TITLE>");
            out.println("<META http-equiv=Content-Type content=\"text/html; charset=utf-8\">");
            out.println("<STYLE>");
            out.println("H1 {FONT-SIZE: 22px; COLOR: white; FONT-FAMILY: Tahoma,Arial,sans-serif; BACKGROUND-COLOR: #525d76}");
            out.println("H2 {FONT-SIZE: 16px; COLOR: white; FONT-FAMILY: Tahoma,Arial,sans-serif; BACKGROUND-COLOR: #525d76}");
            out.println("H3 {FONT-SIZE: 14px; COLOR: white; FONT-FAMILY: Tahoma,Arial,sans-serif; BACKGROUND-COLOR: #525d76}");
            out.println("BODY {COLOR: black; FONT-FAMILY: Tahoma,Arial,sans-serif; BACKGROUND-COLOR: white}");
            out.println("B {COLOR: white; FONT-FAMILY: Tahoma,Arial,sans-serif; BACKGROUND-COLOR: #525d76}");
            out.println("P {FONT-SIZE: 12px; BACKGROUND: white; COLOR: black; FONT-FAMILY: Tahoma,Arial,sans-serif}");
            out.println("A {COLOR: black}");
            out.println("A.name {COLOR: black}");
            out.println("HR {COLOR: #525d76}");
            out.println("</STYLE>");
            out.println("</HEAD>");
            out.println("<BODY>");
            out.println("<H1>HTTP Status 500 - </H1>");
            out.println("<HR noShade SIZE=1>");
            out.println("<P><B>type</B> Exception report</P>");
            out.println("<P><B>message</B> <U></U></P>");
            out.println("<P><B>description</B> <U>The server encountered an internal error () that prevented it from fulfilling this request.</U></P>");
            Throwable t = e;
            int idx = 0;
            while (true)
            {
                if (idx == 0)
                {
                    out.println("<P><B>exception</B>");
                }
                else
                {
                    out.println("<P><B>root cause</B>");
                }
                out.println("<PRE>" + t.toString());
                StackTraceElement[] s = t.getStackTrace();
                for (StackTraceElement ste : s)
                {
                    out.println("	" + ste.toString());
                }
                out.println("</PRE>");
                out.println("<P></P>");

                idx++;

                if (t.getCause() == null)
                {
                    break;
                }
                t = t.getCause();
            }
            out.println("<HR noShade SIZE=1>");
            out.println("<H3>" + SERVER_NAME + "</H3>");
            out.println("</BODY>");
            out.println("</HTML>");
            this.closeConnection();
        }
        catch (IOException ex)
        {
            //
        }
    }

    void unAuthenticated(String url)
    {
        head = HEAD401;
        setHeader("WWW-Authenticate", "Basic realm=\"" + url + "\"");
    }

    public void clear() throws IOException
    {
        status = 0;
        if (inputStreamClose && inputStream != null)
        {
            inputStreamLength = -1;
            inputStream.close();
            inputStream = null;
        }
        this.byteArrayOutputStream = null;
        this.gZIPOutputStream = null;
        this.writer = null;
    }
    
    public void setGZIP(boolean value)
    {
        GZIP = value;
    }

    public boolean isGZIP()
    {
        return GZIP;
    }

    static class Cookie
    {
        public String name;

        public String value;

        public int maxAge;

        public String path;
    }

}
