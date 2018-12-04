package sample;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import webServer.Server;
import webServer.ServerConfig;

public class Main {

	public static void main(String[] Args) throws Exception {
		Server.serverInstance = new Server();

		// String cfg = readToString("config.json");
		// ServerConfig config = JsonUtils.deserialize(cfg, ServerConfig.class);
		ServerConfig config = getConfig();

		Server.serverInstance.init(config);
		// Log.initLog("webServer.log");
		Server.serverInstance.applyConfig();
		Server.serverInstance.startService();
		System.out.println("Server started.");
	}

	public static ServerConfig getConfig() {

		ServerConfig c = new ServerConfig();
		c.port = 81;
		//c.ssl = new ServerConfig.SSL();
		//c.ssl.port = 445;
		//c.ssl.clientKeysFile = "clientKeys.key";
		//c.ssl.keyStorePass = "123456";
		//c.ssl.keyPassword = "123456";

		c.javaCompiler = null;//"file:///C:\\ProgramFiles\\Java\\jdk1.7.0_01\\lib\\tools.jar"
		c.defaultEncodingInput = "utf-8";
		c.defaultEncodingOutput = "utf-8";
		c.sessionExpires = 30000;
		c.fileSystems = new ArrayList<ServerConfig.FileSystem>();

		{
			ServerConfig.FileSystem f = new ServerConfig.FileSystem();
			f.url = "/file/";
			f.rootPath = "/home/zsh/sd/4linux/filesystem/";
			f.allowList = true;
			f.allowUpLoad = true;
			f.allowDel = true;
			f.contentType = ServerConfig.FileSystem.ContentType.auto;
			f.userName = "";
			f.passWord = "";
			c.fileSystems.add(f);
		}
		{
			ServerConfig.FileSystem f = new ServerConfig.FileSystem();
			f.url = "/root/";
			f.rootPath = "/";
			f.allowList = false;
			f.allowUpLoad = false;
			f.allowDel = false;
			f.contentType = ServerConfig.FileSystem.ContentType.auto;
			f.userName = "";
			f.passWord = "";
			c.fileSystems.add(f);
		}
		{
			ServerConfig.FileSystem f = new ServerConfig.FileSystem();
			f.url = "/js/";
			f.rootPath = "/home/zsh/sd/4linux/js/";
			f.allowList = false;
			f.allowUpLoad = false;
			f.allowDel = false;
			f.contentType = ServerConfig.FileSystem.ContentType.auto;
			f.userName = "";
			f.passWord = "";
			c.fileSystems.add(f);
		}
		c.jsps = new ArrayList<ServerConfig.JspProcessor>();
		{
			ServerConfig.JspProcessor j = new ServerConfig.JspProcessor();
			j.url = "/jsp/";
			j.rootPath = "/home/zsh/sd/4linux/jsp/";
			c.jsps.add(j);
		}

		c.servlets = new ArrayList<ServerConfig.Servlet>();
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/";
			s.classFileName = "sample.Root";
			s.isSingleton = false;
			c.servlets.add(s);
		}
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/Control";
			s.classFileName = "sample.Control";
			c.servlets.add(s);
		}
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/ControlV2";
			s.classFileName = "sample.ControlV2";
			c.servlets.add(s);
		}
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/screen.jpg";
			s.classFileName = "sample.ScreenServlet";
			c.servlets.add(s);
		}
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/Screen.jpg";
			s.classFileName = "sample.ScreenServlet";
			c.servlets.add(s);
		}
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/setQuality";
			s.classFileName = "sample.setQualityServlet";
			c.servlets.add(s);
		}
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/MouseAction";
			s.classFileName = "sample.MouseAction";
			c.servlets.add(s);
		}
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/KeyAction";
			s.classFileName = "sample.KeyAction";
			c.servlets.add(s);
		}
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/MessageBoxAction";
			s.classFileName = "sample.MessageboxAction";
			c.servlets.add(s);
		}
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/ClipBoardAction";
			s.classFileName = "sample.ClipBoardAction";
			c.servlets.add(s);
		}
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/processList";
			s.classFileName = "sample.ProcessListAction";
			c.servlets.add(s);
		}
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/processKill";
			s.classFileName = "sample.ProcessKillAction";
			c.servlets.add(s);
		}
		{
			ServerConfig.Servlet s = new ServerConfig.Servlet();
			s.url = "/Login";
			s.classFileName = "sample.LoginServlet";
			c.servlets.add(s);
		}
		return c;
	}

	public static String readToString(String fileName) {
		String encoding = "UTF-8";
		File file = new File(fileName);
		Long filelength = file.length();
		byte[] filecontent = new byte[filelength.intValue()];
		try {
			FileInputStream in = new FileInputStream(file);
			in.read(filecontent);
			in.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		try {
			return new String(filecontent, encoding);
		} catch (UnsupportedEncodingException e) {
			System.err.println("The OS does not support " + encoding);
			e.printStackTrace();
			return null;
		}
	}
}
