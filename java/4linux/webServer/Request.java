package webServer;

import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.util.HashMap;
import java.util.Locale;

public final class Request
{
    public final static int METHOD_GET = 0;

    public final static int METHOD_POST = 1;

    public final static int PROTOCOL_HTTP = 0;

    public final static int PROTOCOL_OTHER = 1;

    Server server;

    int method;

    String path;

    int protocol;

    String version;

    HashMap<String, String> cookies = new HashMap<String, String>();

    HashMap<String, String> HTTPParam = new HashMap<String, String>();

    HashMap<String, String> GETParam = new HashMap<String, String>();

    HashMap<String, String> POSTParam = new HashMap<String, String>();

    HashMap<String, MultipartData> MultipartData = new HashMap<String, MultipartData>();

    SocketAddress remoteAddress;

    Session session;

    String contentType;

    int contentLength = 0;

    boolean sessionIsNullCurrent;

    boolean sessionIsNullFirstTime;

    boolean GZIP = false;

    Locale locale;

    String charset;

    String authorizationUsername;

    String authorizationPassword;

    boolean closeConnection = false;

    public int getMethod()
    {
        return method;
    }

    void setMethod(int i)
    {
        method = i;
    }

    public String getPath()
    {
        return path;
    }

    void setPath(String string)
    {
        path = string;
    }

    public String getGETParam(String name)
    {
        return GETParam.get(name);
    }

    public String getParam(String name) {
        String ret = POSTParam.get(name);
        if (ret == null) {
            ret = GETParam.get(name);
        }
        return ret;
    }

    void setGETParam(String name, String value)
    {
        GETParam.put(name, value);
    }

    public MultipartData getMultipartData(String name)
    {
        return MultipartData.get(name);
    }

    void setMultipartData(String name, MultipartData value)
    {
        MultipartData.put(name, value);
    }

    public String getPOSTParam(String name)
    {
        return POSTParam.get(name);
    }

    void setPOSTParam(String name, String value)
    {
        POSTParam.put(name, value);
    }

    public String getHTTPParam(String name)
    {
        return HTTPParam.get(name);
    }

    void setHTTPParam(String name, String value)
    {
        HTTPParam.put(name, value);
    }

    public Session getSession(boolean create)
    {
        if (create && sessionIsNullCurrent)
        {
            session = new Session(server);

            sessionIsNullCurrent = false;
       }
        return session;
    }

    public Session getSession()
    {
        return getSession(true);
    }

    void setSession(Session session)
    {
        this.session = session;
    }

    public String getCookie(String name)
    {
        return cookies.get(name);
    }

    void setCookie(String name, String value)
    {
        cookies.put(name, value);
    }

    public int getProtocol()
    {
        return protocol;
    }

    void setProtocol(int i)
    {
        protocol = i;
    }

    public String getHTTPVersion()
    {
        return version;
    }

    void setHTTPVersion(String string)
    {
        version = string;
    }

    public String getRemoteHostName()
    {
        return ((InetSocketAddress) remoteAddress).getAddress().getHostName();
    }

    public String getRemoteAddr()
    {
        return ((InetSocketAddress) remoteAddress).getAddress().getHostAddress();
    }

    public int getRemotePort()
    {
        return ((InetSocketAddress) remoteAddress).getPort();
    }

    void setRemoteAddress(SocketAddress address)
    {
        remoteAddress = address;
    }

    public String getContentType()
    {
        return contentType;
    }

    void setContentType(String string)
    {
        contentType = string;
    }

    public Locale getLocale()
    {
        return locale;
    }

    void setLocale(Locale locale)
    {
        this.locale = locale;
    }

    public String getCharset()
    {
        return charset;
    }

    void setCharset(String charset)
    {
        this.charset = charset;
    }
}
