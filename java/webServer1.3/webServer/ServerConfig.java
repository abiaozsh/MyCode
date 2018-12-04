package webServer;

import java.io.Serializable;
import java.util.List;

public class ServerConfig implements Serializable
{

	public int port;

	public static class SSL implements Serializable
	{
		public int port;

		public String clientKeysFile;

		public String keyStorePass;

		public String keyPassword;
	}

	public SSL ssl;

	public int sessionExpires;

	public String javaCompiler;

	public String defaultEncodingInput;
	public String defaultEncodingOutput;

	public static class FileSystem implements Serializable
	{
		public String url = null;

		public String rootPath = null;

		public boolean allowList = false;

		public boolean allowUpLoad = false;

		public boolean allowDel = false;

		public static enum ContentType{
			auto,
			forceDownload,
			forceText
		}
		public ContentType contentType;

		public String userName = null;

		public String passWord = null;
	}
	
	public List<FileSystem> fileSystems;

	public static class JspProcessor implements Serializable
	{
		public String url = null;

		public String rootPath = null;
	}

	public List<JspProcessor> jsps;

	public static class Servlet implements Serializable
	{
		public String classFileName = null;

		public String url = null;

		public boolean isSingleton = false;
	}
	
	public List<Servlet> servlets;

}
