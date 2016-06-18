package webServer;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.Serializable;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Text;

public class ServerConfig
{
	private Document doc;

	public int getPort()
	{
		try
		{
			Node root = getRoot();
			Node port = getNode(root, "port");
			return Integer.parseInt(getValueNull(port).trim());
		}
		catch (Exception ex)
		{
			return 80;
		}
	}

	public void setPort(int val)
	{
		Node root = getRoot();
		Node port = getNode(root, "port");
		setValue(port, Integer.toString(val));
	}

	public boolean getSSLactive()
	{
		try
		{
			Node root = getRoot();
			Node SSL = getNode(root, "SSL");
			Node SSLactive = getNode(SSL, "active");
			return getBoolean(SSLactive);
		}
		catch (Exception ex)
		{
			return false;
		}
	}

	public void setSSLactive(boolean val)
	{
		Node root = getRoot();
		Node SSL = getNode(root, "SSL");
		Node SSLactive = getNode(SSL, "active");
		setBoolean(SSLactive, val);
	}

	public int getSSLport()
	{
		try
		{
			Node root = getRoot();
			Node SSL = getNode(root, "SSL");
			Node SSLport = getNode(SSL, "port");
			return Integer.parseInt(getValueNull(SSLport).trim());
		}
		catch (Exception ex)
		{
			return 443;
		}
	}

	public void setSSLport(int val)
	{
		Node root = getRoot();
		Node SSL = getNode(root, "SSL");
		Node SSLport = getNode(SSL, "port");
		setValue(SSLport, Integer.toString(val));
	}

	public String getSSLclientKeysFile()
	{
		try
		{
			Node root = getRoot();
			Node SSL = getNode(root, "SSL");
			Node SSLclientKeysFile = getNode(SSL, "clientKeysFile");
			return getValueNull(SSLclientKeysFile);
		}
		catch (Exception ex)
		{
			return "";
		}
	}

	public void setSSLclientKeysFile(String val)
	{
		Node root = getRoot();
		Node SSL = getNode(root, "SSL");
		Node SSLclientKeysFile = getNode(SSL, "clientKeysFile");
		setValue(SSLclientKeysFile, val);
	}

	public String getSSLkeyStorePass()
	{
		try
		{
			Node root = getRoot();
			Node SSL = getNode(root, "SSL");
			Node SSLkeyStorePass = getNode(SSL, "keyStorePass");
			return getValueNull(SSLkeyStorePass);
		}
		catch (Exception ex)
		{
			return "";
		}
	}

	public void setSSLkeyStorePass(String val)
	{
		Node root = getRoot();
		Node SSL = getNode(root, "SSL");
		Node SSLkeyStorePass = getNode(SSL, "keyStorePass");
		setValue(SSLkeyStorePass, val);
	}

	public String getSSLkeyPassword()
	{
		try
		{
			Node root = getRoot();
			Node SSL = getNode(root, "SSL");
			Node SSLkeyPassword = getNode(SSL, "keyPassword");
			return getValueNull(SSLkeyPassword);
		}
		catch (Exception ex)
		{
			return "";
		}
	}

	public void setSSLkeyPassword(String val)
	{
		Node root = getRoot();
		Node SSL = getNode(root, "SSL");
		Node SSLkeyPassword = getNode(SSL, "keyPassword");
		setValue(SSLkeyPassword, val);
	}

	public int getSessionExpires()
	{
		try
		{
			Node root = getRoot();
			Node sessionExpires = getNode(root, "sessionExpires");
			return Integer.parseInt(getValueNull(sessionExpires).trim());
		}
		catch (Exception ex)
		{
			return 300;//5min
		}
	}

	public void setSessionExpires(int val)
	{
		Node root = getRoot();
		Node sessionExpires = getNode(root, "sessionExpires");
		setValue(sessionExpires, Integer.toString(val));
	}

	public boolean getConfigPageEnabled()
	{
		try
		{
			Node root = getRoot();
			Node configPage = getNode(root, "configPage");
			Node enabled = getNode(configPage, "enabled");
			return getBoolean(enabled);
		}
		catch (Exception ex)
		{
			return false;
		}
	}

	public void setConfigPageEnabled(boolean val)
	{
		Node root = getRoot();
		Node configPage = getNode(root, "configPage");
		Node enabled = getNode(configPage, "enabled");
		setBoolean(enabled, val);
	}

	public String getConfigPageUrl()
	{
		try
		{
			Node root = getRoot();
			Node configPage = getNode(root, "configPage");
			Node url = getNode(configPage, "url");
			return getValueNull(url);
		}
		catch (Exception ex)
		{
			return "";
		}
	}

	public void setConfigPageUrl(String val)
	{
		Node root = getRoot();
		Node configPage = getNode(root, "configPage");
		Node url = getNode(configPage, "url");
		setValue(url, val);
	}

	public String getConfigUserName()
	{
		try
		{
			Node root = getRoot();
			Node configPage = getNode(root, "configPage");
			Node userName = getNode(configPage, "userName");
			return getValueNull(userName);
		}
		catch (Exception ex)
		{
			return "";
		}
	}

	public void setConfigUserName(String val)
	{
		Node root = getRoot();
		Node configPage = getNode(root, "configPage");
		Node userName = getNode(configPage, "userName");
		setValue(userName, val);
	}

	public String getConfigPassWord()
	{
		try
		{
			Node root = getRoot();
			Node configPage = getNode(root, "configPage");
			Node passWord = getNode(configPage, "passWord");
			return getValueNull(passWord);
		}
		catch (Exception ex)
		{
			return "";
		}
	}

	public void setConfigPassWord(String val)
	{
		Node root = getRoot();
		Node configPage = getNode(root, "configPage");
		Node passWord = getNode(configPage, "passWord");
		setValue(passWord, val);
	}

	public String getLogFile()
	{
		try
		{
			Node root = getRoot();
			Node logFile = getNode(root, "logFile");
			return getValueNull(logFile);
		}
		catch (Exception ex)
		{
			return "";
		}
	}

	public void setLogFile(String val)
	{
		Node root = getRoot();
		Node logFile = getNode(root, "logFile");
		setValue(logFile, val);
	}

	public String getJavaCompiler()
	{
		try
		{
			Node root = getRoot();
			Node javaCompiler = getNode(root, "javaCompilerPath");
			return getValueNull(javaCompiler);
		}
		catch (Exception ex)
		{
			return "";
		}
	}

	public void setJavaCompiler(String val)
	{
		Node root = getRoot();
		Node javaCompiler = getNode(root, "javaCompilerPath");
		setValue(javaCompiler, val);
	}

	public String getDefaultEncoding()
	{
		try
		{
			Node root = getRoot();
			Node defaultEncoding = getNode(root, "defaultEncoding");
			return getValueNull(defaultEncoding);
		}
		catch (Exception ex)
		{
			return "utf-8";
		}
	}

	public void setDefaultEncoding(String val)
	{
		Node root = getRoot();
		Node defaultEncoding = getNode(root, "defaultEncoding");
		setValue(defaultEncoding, val);
	}

	public ArrayList<FileSystem> getFileSystems()
	{
		try
		{
			Node root = getRoot();
			ArrayList<FileSystem> fileSystems = new ArrayList<FileSystem>();
			for (int i = 0; i < root.getChildNodes().getLength(); i++)
			{
				Node nFs = root.getChildNodes().item(i);
				if ("fileSystem".equals(nFs.getNodeName()))
				{
					FileSystem cfs = new FileSystem();
					cfs.rootPath = getValue(getNode(nFs, "rootPath"));
					cfs.url = getValue(getNode(nFs, "url"));
					cfs.allowList = getBoolean(getNode(nFs, "allowList"));
					cfs.allowUpLoad = getBoolean(getNode(nFs, "allowUpLoad"));
					cfs.allowDel = getBoolean(getNode(nFs, "allowDel"));
					cfs.isDownload = getBoolean(getNode(nFs, "isDownload"));
					cfs.userName = getValue(getNode(nFs, "userName"));
					cfs.passWord = getValue(getNode(nFs, "passWord"));
					fileSystems.add(cfs);
				}
			}

			return fileSystems;
		}
		catch (Exception ex)
		{
			return new ArrayList<FileSystem>();
		}
	}

	public void updateFileSystems(int idx, FileSystem item)
	{
		Node root = getRoot();
		for (int i = 0; i < root.getChildNodes().getLength(); i++)
		{
			Node nFs = root.getChildNodes().item(i);
			if ("fileSystem".equals(nFs.getNodeName()))
			{
				if (idx == 0)
				{
					setValue(getNode(nFs, "rootPath"), item.rootPath);
					setValue(getNode(nFs, "url"), item.url);
					setBoolean(getNode(nFs, "allowList"), item.allowList);
					setBoolean(getNode(nFs, "allowUpLoad"), item.allowUpLoad);
					setBoolean(getNode(nFs, "allowDel"), item.allowDel);
					setBoolean(getNode(nFs, "isDownload"), item.isDownload);
					setValue(getNode(nFs, "userName"), item.userName);
					setValue(getNode(nFs, "passWord"), item.passWord);
					return;
				}
				idx--;
			}
		}
	}

	public void deleteFileSystems(int idx)
	{
		Node root = getRoot();
		for (int i = 0; i < root.getChildNodes().getLength(); i++)
		{
			Node nFs = root.getChildNodes().item(i);
			if ("fileSystem".equals(nFs.getNodeName()))
			{
				if (idx == 0)
				{
					root.removeChild(nFs);
					return;
				}
				idx--;
			}
		}
	}

	public void insertFileSystems(FileSystem item)
	{
		Node root = getRoot();
		Node nFs = doc.createElement("fileSystem");
		root.appendChild(nFs);

		setValue(getNode(nFs, "rootPath"), item.rootPath);
		setValue(getNode(nFs, "url"), item.url);
		setBoolean(getNode(nFs, "allowList"), item.allowList);
		setBoolean(getNode(nFs, "allowUpLoad"), item.allowUpLoad);
		setBoolean(getNode(nFs, "allowDel"), item.allowDel);
		setBoolean(getNode(nFs, "isDownload"), item.isDownload);
		setValue(getNode(nFs, "userName"), item.userName);
		setValue(getNode(nFs, "passWord"), item.passWord);
		return;
	}

	public ArrayList<JspProcessor> getJspProcessors()
	{
		try
		{
			Node root = getRoot();
			ArrayList<JspProcessor> jspProcessors = new ArrayList<JspProcessor>();
			for (int i = 0; i < root.getChildNodes().getLength(); i++)
			{
				Node nJsp = root.getChildNodes().item(i);
				if ("jsp".equals(nJsp.getNodeName()))
				{
					JspProcessor jsp = new JspProcessor();
					jsp.rootPath = getValue(getNode(nJsp, "rootPath"));
					jsp.url = getValue(getNode(nJsp, "url"));

					jspProcessors.add(jsp);
				}
			}
			return jspProcessors;
		}
		catch (Exception ex)
		{
			return new ArrayList<JspProcessor>();
		}
	}

	public void updateJspProcessors(int idx, JspProcessor item)
	{
		Node root = getRoot();
		for (int i = 0; i < root.getChildNodes().getLength(); i++)
		{
			Node nJsp = root.getChildNodes().item(i);
			if ("jsp".equals(nJsp.getNodeName()))
			{
				if (idx == 0)
				{
					setValue(getNode(nJsp, "rootPath"), item.rootPath);
					setValue(getNode(nJsp, "url"), item.url);

					return;
				}
				idx--;
			}
		}
	}

	public void deleteJspProcessors(int idx)
	{
		Node root = getRoot();
		for (int i = 0; i < root.getChildNodes().getLength(); i++)
		{
			Node nJsp = root.getChildNodes().item(i);
			if ("jsp".equals(nJsp.getNodeName()))
			{
				if (idx == 0)
				{
					root.removeChild(nJsp);
					return;
				}
				idx--;
			}
		}
	}

	public void insertJspProcessors(JspProcessor item)
	{
		Node root = getRoot();
		Node nJsp = doc.createElement("jsp");
		root.appendChild(nJsp);

		setValue(getNode(nJsp, "rootPath"), item.rootPath);
		setValue(getNode(nJsp, "url"), item.url);

		return;
	}

	public ArrayList<Servlet> getServlets()
	{
		try
		{
			Node root = getRoot();
			ArrayList<Servlet> servlets = new ArrayList<Servlet>();
			for (int i = 0; i < root.getChildNodes().getLength(); i++)
			{
				Node nSv = root.getChildNodes().item(i);
				if ("servlet".equals(nSv.getNodeName()))
				{
					Servlet servlet = new Servlet();
					servlet.url = getValue(getNode(nSv, "url"));
					servlet.classFileName = getValue(getNode(nSv, "class"));
					servlet.isSingleton = getBoolean(getNode(nSv, "isSingleton"));

					servlets.add(servlet);
				}
			}
			return servlets;
		}
		catch (Exception ex)
		{
			return new ArrayList<Servlet>();
		}
	}

	public void updateServlets(int idx, Servlet item)
	{
		Node root = getRoot();
		for (int i = 0; i < root.getChildNodes().getLength(); i++)
		{
			Node nSv = root.getChildNodes().item(i);
			if ("servlet".equals(nSv.getNodeName()))
			{
				if (idx == 0)
				{
					setValue(getNode(nSv, "url"), item.url);
					setValue(getNode(nSv, "class"), item.classFileName);
					setBoolean(getNode(nSv, "isSingleton"), item.isSingleton);
					return;
				}
				idx--;
			}
		}
	}

	public void deleteServlets(int idx)
	{
		Node root = getRoot();
		for (int i = 0; i < root.getChildNodes().getLength(); i++)
		{
			Node nSv = root.getChildNodes().item(i);
			if ("servlet".equals(nSv.getNodeName()))
			{
				if (idx == 0)
				{
					root.removeChild(nSv);
					return;
				}
				idx--;
			}
		}
	}

	public void insertServlets(Servlet item)
	{
		Node root = getRoot();
		Node nSv = doc.createElement("servlet");
		root.appendChild(nSv);
		setValue(getNode(nSv, "url"), item.url);
		setValue(getNode(nSv, "class"), item.classFileName);
		setBoolean(getNode(nSv, "isSingleton"), item.isSingleton);
		return;
	}

	private Node getRoot()
	{
		NodeList nl;
		nl = doc.getChildNodes();
		Node root = nl.item(0);
		if ("config".equals(root.getNodeName()))
		{
			return root;
		}
		else
		{
			return null;
		}
	}

	private Node getNode(Node node, String name)
	{
		for (int i = 0; i < node.getChildNodes().getLength(); i++)
		{
			Node child = node.getChildNodes().item(i);
			if (name.equals(child.getNodeName()))
			{
				return child;
			}
		}
		Node newChild = doc.createElement(name);
		node.appendChild(newChild);
		return newChild;
	}

	public static class FileSystem implements Serializable
	{
		String url = null;

		String rootPath = null;

		boolean allowList = false;

		boolean allowUpLoad = false;

		boolean allowDel = false;

		boolean isDownload = true;

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

		boolean isSingleton = false;
	}

	void LoadConfig(String configFile)
	{
		try
		{
			File f = new File(configFile);
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			DocumentBuilder builder = factory.newDocumentBuilder();
			doc = builder.parse(f);
		}
		catch (Exception e)
		{
			Log.log(e);
		}
	}

	void SaveConfig(String configFile)
	{
		try
		{
			TransformerFactory tf = TransformerFactory.newInstance();
			Transformer transformer = tf.newTransformer();
			DOMSource source = new DOMSource(doc);
			transformer.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
			transformer.setOutputProperty(OutputKeys.INDENT, "yes");// 设置文档的换行与缩进
			FileOutputStream fos = new FileOutputStream(configFile);
			OutputStreamWriter osw = new OutputStreamWriter(fos, "UTF-8");
			StreamResult result = new StreamResult(osw);
			transformer.transform(source, result);
			osw.close();
			fos.close();
		}
		catch (Exception e)
		{
			Log.log(e);
		}
	}

	private String getValueNull(Node n)
	{
		return n.getFirstChild().getNodeValue();
	}

	private String getValue(Node n)
	{
		try
		{
			return n.getFirstChild().getNodeValue();
		}
		catch (Exception ex)
		{
			return "";
		}
	}

	private void setValue(Node n, String val)
	{
		if (val == null) val = "";
		if (n.getFirstChild() == null)
		{
			Text t = doc.createTextNode(val);
			n.appendChild(t);
		}
		else
		{
			n.getFirstChild().setNodeValue(val);
		}
	}

	private boolean getBoolean(Node n)
	{
		if ("true".equals(getValueNull(n).trim()))
		{
			return true;
		}
		return false;
	}

	private void setBoolean(Node n, boolean val)
	{
		if (val)
		{
			setValue(n, "true");
		}
		else
		{
			setValue(n, "false");
		}
	}

}
