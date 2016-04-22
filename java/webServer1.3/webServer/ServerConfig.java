package webServer;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Serializable;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class ServerConfig
{
	public String configFile;

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

	public String logFile = "";

	public ArrayList<FileSystem> fileSystems = new ArrayList<FileSystem>();

	public ArrayList<JspProcessor> jspProcessors = new ArrayList<JspProcessor>();

	public ArrayList<Servlet> servlets = new ArrayList<Servlet>();

	public static class FileSystem implements Serializable
	{
		String url = null;

		String rootPath = null;

		boolean allowList = false;

		boolean allowUpLoad = false;

		boolean allowDel = false;

		String userName = null;

		String passWord = null;
	}

	public static class JspProcessor implements Serializable
	{
		String url = null;

		String rootPath = null;
	}

	public static class Servlet implements Serializable
	{
		String classFileName = null;

		String url = null;
	}

	void SaveConfig() throws IOException
	{

		File f = new File(configFile);

		FileOutputStream fos = new FileOutputStream(f);
		out = new OutputStreamWriter(fos);

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
		write(" <logFile>", logFile, "</logFile>");
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
		out.flush();
		out.close();
		fos.close();
		out = null;
	}

	private OutputStreamWriter out;

	private void write(String s1, int s2, String s3) throws IOException
	{
		out.write(s1 + s2 + s3 + "\r\n");
	}

	private void write(String s1, boolean s2, String s3) throws IOException
	{
		out.write(s1 + s2 + s3 + "\r\n");
	}

	private void write(String s1, String s2, String s3) throws IOException
	{
		if (s2 == null)
		{
			out.write(s1 + s3 + "\r\n");
		}
		else
		{
			out.write(s1 + s2 + s3 + "\r\n");
		}
	}

	private void write(String s) throws IOException
	{
		out.write(s + "\r\n");
	}

	void readConfigFromFile(String configFile)
	{
		try
		{
			File f = new File(configFile);
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder builder = factory.newDocumentBuilder();
			Document doc = builder.parse(f);
			NodeList nl;
			nl = doc.getChildNodes();
			Node root = nl.item(0);

			if ("config".equals(root.getNodeName()))
			{
				for (int i = 0; i < root.getChildNodes().getLength(); i++)
				{
					Node child = root.getChildNodes().item(i);
					if ("port".equals(child.getNodeName()))
					{
						port = Integer.parseInt(getValue(child));
					}
					if ("SSL".equals(child.getNodeName()))
					{
						for (int j = 0; j < child.getChildNodes().getLength(); j++)
						{
							Node SSLchild = child.getChildNodes().item(j);

							if ("active".equals(SSLchild.getNodeName()))
							{
								SSLactive = getBoolean(SSLchild);
							}
							if ("port".equals(SSLchild.getNodeName()))
							{
								SSLport = Integer.parseInt(getValue(SSLchild));
							}
							if ("clientKeysFile".equals(SSLchild.getNodeName()))
							{
								SSLclientKeysFile = getValue(SSLchild);
							}
							if ("keyStorePass".equals(SSLchild.getNodeName()))
							{
								SSLkeyStorePass = getValue(SSLchild);
							}
							if ("keyPassword".equals(SSLchild.getNodeName()))
							{
								SSLkeyPassword = getValue(SSLchild);
							}
						}
					}
					if ("javaCompilerPath".equals(child.getNodeName()))
					{
						String path = getValue(child);
						javaCompiler = path;
					}
					if ("defaultEncoding".equals(child.getNodeName()))
					{
						defaultEncoding = getValue(child);
					}
					if ("sessionExpires".equals(child.getNodeName()))
					{
						sessionExpires = Integer.parseInt(getValue(child));
					}
					if ("logFile".equals(child.getNodeName()))
					{
						logFile = getValue(child);
					}
					if ("configPage".equals(child.getNodeName()))
					{
						for (int j = 0; j < child.getChildNodes().getLength(); j++)
						{
							Node nFs = child.getChildNodes().item(j);
							if ("enabled".equals(nFs.getNodeName()))
							{
								configPageEnabled = getBoolean(nFs);
							}
							if ("url".equals(nFs.getNodeName()))
							{
								configPage = getValue(nFs);
							}
							if ("userName".equals(nFs.getNodeName()))
							{
								configUserName = getValue(nFs);
							}
							if ("passWord".equals(nFs.getNodeName()))
							{
								configPassWord = getValue(nFs);
							}
						}
					}
					if ("fileSystem".equals(child.getNodeName()))
					{
						FileSystem cfs = new FileSystem();
						for (int j = 0; j < child.getChildNodes().getLength(); j++)
						{
							Node nFs = child.getChildNodes().item(j);
							if ("rootPath".equals(nFs.getNodeName()))
							{
								cfs.rootPath = getValue(nFs);
							}
							if ("url".equals(nFs.getNodeName()))
							{
								cfs.url = getValue(nFs);
							}
							if ("allowList".equals(nFs.getNodeName()))
							{
								cfs.allowList = getBoolean(nFs);
							}
							if ("allowUpLoad".equals(nFs.getNodeName()))
							{
								cfs.allowUpLoad = getBoolean(nFs);
							}
							if ("allowDel".equals(nFs.getNodeName()))
							{
								cfs.allowDel = getBoolean(nFs);
							}
							if ("userName".equals(nFs.getNodeName()))
							{
								cfs.userName = getValue(nFs);
							}
							if ("passWord".equals(nFs.getNodeName()))
							{
								cfs.passWord = getValue(nFs);
							}
						}
						fileSystems.add(cfs);
					}
					if ("jsp".equals(child.getNodeName()))
					{
						JspProcessor jsp = new JspProcessor();
						for (int j = 0; j < child.getChildNodes().getLength(); j++)
						{
							Node nJsp = child.getChildNodes().item(j);
							if ("rootPath".equals(nJsp.getNodeName()))
							{
								jsp.rootPath = getValue(nJsp);
							}
							if ("url".equals(nJsp.getNodeName()))
							{
								jsp.url = getValue(nJsp);
							}
						}
						jspProcessors.add(jsp);
					}
					if ("servlet".equals(child.getNodeName()))
					{
						Servlet cs = new Servlet();
						for (int j = 0; j < child.getChildNodes().getLength(); j++)
						{
							Node nJsp = child.getChildNodes().item(j);
							if ("url".equals(nJsp.getNodeName()))
							{
								cs.url = getValue(nJsp);
							}
							if ("class".equals(nJsp.getNodeName()))
							{
								cs.classFileName = getValue(nJsp);
							}
						}
						servlets.add(cs);
					}
				}
			}
		}
		catch (Exception e)
		{
			Log.log(e);
		}
	}

	private String getValue(Node n)
	{
		if (n.getFirstChild() == null)
		{
			return null;

		}
		else
		{
			return n.getFirstChild().getNodeValue();
		}
	}

	private boolean getBoolean(Node n)
	{
		if ("true".equals(getValue(n)))
		{
			return true;
		}
		return false;
	}

}
