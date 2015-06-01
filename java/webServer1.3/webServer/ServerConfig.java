package webServer;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;

public class ServerConfig
{
	public int port = 80;

	public boolean SSLactive = false;

	public int SSLport = 443;

	public String SSLclientKeysFile = null;

	public String SSLkeyStorePass = null;

	public String SSLkeyPassword = null;

	public String javaCompiler = null;

	public String defaultEncoding = "utf-8";

	public int sessionExpires = 300;// 5min

	public boolean configPageEnabled = false;

	public String configPage = null;

	public String configUserName = null;

	public String configPassWord = null;

	public ArrayList<FileSystem> fileSystems = new ArrayList<FileSystem>();

	public ArrayList<JspProcessor> jspProcessors = new ArrayList<JspProcessor>();

	public ArrayList<Servlet> servlets = new ArrayList<Servlet>();

	private PrintWriter out;

	String MakeXml()
	{
		StringWriter sw = new StringWriter();
		out = new PrintWriter(sw);

		write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		write("<config>");
		write(" ");
		write(" <port>", port, "</port>");
		write(" ");
		write(" <SSL>");
		write("  <active>", SSLactive, "</active>");
		write("  <port>", SSLport, "</port>");
		write("  <clientKeysFile>", SSLclientKeysFile, "</clientKeysFile>");
		write("  <keyStorePass>", SSLkeyStorePass, "</keyStorePass>");
		write("  <keyPassword>", SSLkeyPassword, "</keyPassword>");
		write(" </SSL>");
		write(" ");
		write(" <javaCompilerPath>", javaCompiler, "</javaCompilerPath>");
		write(" ");
		write(" <defaultEncoding>", defaultEncoding, "</defaultEncoding>");
		write(" ");
		write(" <sessionExpires>", sessionExpires, "</sessionExpires>");
		write(" ");
		write(" <configPage>");
		write("  <enabled>", configPageEnabled, "</enabled>");
		write("  <url>", configPage, "</url>");
		write("  <userName>", configUserName, "</userName>");
		write("  <passWord>", configPassWord, "</passWord>");
		write(" </configPage>");
		write(" ");
		for (FileSystem cfs : fileSystems)
		{
			write(" <fileSystem>");
			write("  <url>", cfs.url, "</url>");
			write("  <rootPath>", cfs.rootPath, "</rootPath>");
			write("  <allowList>", cfs.allowList, "</allowList>");
			write("  <allowUpLoad>", cfs.allowUpLoad, "</allowUpLoad>");
			write("  <allowDel>", cfs.allowDel, "</allowDel>");
			write("  <userName>", cfs.userName, "</userName>");
			write("  <passWord>", cfs.passWord, "</passWord>");
			write(" </fileSystem>");
			write(" ");
		}
		for (JspProcessor cjp : jspProcessors)
		{
			write(" <jsp>");
			write("  <url>", cjp.url, "</url>");
			write("  <rootPath>", cjp.rootPath, "</rootPath>");
			write(" </jsp>");
			write(" ");
		}
		for (Servlet cs : servlets)
		{
			write(" <servlet>");
			write("  <url>", cs.url, "</url>");
			write("  <class>", cs.classFileName, "</class>");
			write(" </servlet>");
			write(" ");
		}
		write("</config>");
		out.close();

		out = null;

		return new String(sw.getBuffer().toString());
	}

	private void write(String s)
	{
		out.println(s);
	}

	private void write(String s1, int s2, String s3)
	{
		out.println(s1 + s2 + s3);
	}

	private void write(String s1, boolean s2, String s3)
	{
		out.println(s1 + s2 + s3);
	}

	private void write(String s1, String s2, String s3)
	{
		if (s2 == null)
		{
			out.println(s1 + s3);
		}
		else
		{
			out.println(s1 + s2 + s3);
		}
	}

	static class FileSystem
	{
		String url = null;

		String rootPath = null;

		boolean allowList = false;

		boolean allowUpLoad = false;

		boolean allowDel = false;

		String userName = null;

		String passWord = null;
	}

	static class JspProcessor
	{
		String url = null;

		String rootPath = null;
	}

	static class Servlet
	{
		String classFileName = null;

		String url = null;
	}
}
