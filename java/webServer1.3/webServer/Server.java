package webServer;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.lang.reflect.Method;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.net.URL;
import java.net.URLClassLoader;
import java.security.KeyStore;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Timer;

import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLServerSocketFactory;

public final class Server
{

	final static String SEP = File.separator;

	public static ClassLoader classloader = Server.class.getClassLoader();

	public static Server serverInstance = null;

	ServerConfig currentConfig = null;

	ArrayList<JspProcessor> jspProcessors = null;

	ArrayList<FileSystem> fileSystems = null;

	Monitor monitor = null;

	HashMap<String, ServletPack> servlets = null;

	HashMap<String, Object> applicationData = null;

	HashMap<String, Session> sessionPooling = null;

	Method javaCompiler = null;

	ServerThread st;

	ServerThread sst;

	Timer sessionCleaner;

	public RequestDispatcher getRequestDispatcher(String target)
	{
		return new RequestDispatcher(target, this);
	}

	public Object getApplicationData(String id)
	{
		synchronized (applicationData)
		{
			return applicationData.get(id);
		}
	}

	public void setApplicationData(String id, Object info)
	{
		synchronized (applicationData)
		{
			applicationData.put(id, info);
		}
	}

	Session getSession(String id)
	{
		synchronized (sessionPooling)
		{
			return sessionPooling.get(id);
		}
	}

	void setSession(String id, Session session)
	{
		synchronized (sessionPooling)
		{
			sessionPooling.put(id, session);
		}
	}

	void init(String configFile)
	{
		sessionPooling = new HashMap<String, Session>();
		applicationData = new HashMap<String, Object>();

		currentConfig = new ServerConfig();
		currentConfig.readConfigFromFile(configFile);
		currentConfig.configFile = configFile;
	}

	void applyConfig()
	{

		jspProcessors = new ArrayList<JspProcessor>();
		for (ServerConfig.JspProcessor cjp : currentConfig.jspProcessors)
		{
			jspProcessors.add(new JspProcessor(cjp.url, cjp.rootPath));
		}

		fileSystems = new ArrayList<FileSystem>();
		for (ServerConfig.FileSystem cfs : currentConfig.fileSystems)
		{
			fileSystems.add(new FileSystem(cfs.url, cfs.rootPath, cfs.allowList, cfs.allowUpLoad, cfs.allowDel, cfs.userName, cfs.passWord));
		}

		if (currentConfig.javaCompiler != null)
		{
			String path = currentConfig.javaCompiler;
			if (path.startsWith("file:///"))
			{
				if (path.substring(8).indexOf(":") >= 0 || path.substring(8).startsWith("/"))
				{
					// ok
				}
				else
				{
					path = "file:///" + System.getProperty("user.dir") + "/" + path.substring(8);
					path = path.replaceAll("[\\\\]", "/");
				}
			}
			else if (path.startsWith("ftp://"))
			{
				// ok
			}
			else if (path.startsWith("http://"))
			{
				// ok
			}
			else
			{
				if (path.indexOf(":") < 0)
				{
					path = "file:///" + System.getProperty("user.dir") + "/" + path;
				}
				else
				{
					path = "file:///" + path;
				}
				path = path.replaceAll("[\\\\]", "/");
			}
			try
			{
				URLClassLoader ucl = new URLClassLoader(new URL[] { new URL(path) });
				Class<?> c = ucl.loadClass("com.sun.tools.javac.Main");
				Exception jspLoadException = null;
				try
				{
					javaCompiler = c.getMethod("compile", String[].class, PrintWriter.class);
				}
				catch (Exception e)
				{
					javaCompiler = null;
					jspLoadException = e;
				}
				ucl.close();
				if (javaCompiler == null)
				{
					throw new Exception("Failed to load JavaCompiler, JSP function will not work.", jspLoadException);
				}
			}
			catch (Exception e)
			{
				Log.log(e);
			}

		}

		servlets = new HashMap<String, ServletPack>();

		for (ServerConfig.Servlet sc : currentConfig.servlets)
		{
			String classFileName = sc.classFileName;
			try
			{
				ServletPack sp = new ServletPack();

				sp.clazz = classloader.loadClass(classFileName);

				//TODO ?
				//sp.clazz.newInstance();

				sp.url = sc.url;

				servlets.put(sp.url, sp);
			}
			catch (ClassNotFoundException e)
			{
				Log.log("class " + classFileName + " not found.");
			}
			catch (Exception e)
			{
				Log.log(e);
			}
		}

		if (currentConfig.configPageEnabled && currentConfig.configPage != null)
		{
			ServletPack sp = new ServletPack();
			sp.clazz = ConfigServlet.class;
			sp.url = currentConfig.configPage;
			servlets.put(currentConfig.configPage, sp);
		}
	}

	void startService()
	{
		monitor = new Monitor(this);
		monitor.start();
		monitor.setName("Service Monitor");
		sessionCleaner = new Timer("Session Cleaner");
	}

	void restart()
	{
		synchronized (monitor)
		{
			monitor.notify();
		}
		Log.log("server reseted");
	}

	void shutdown()
	{
		monitor.stop = true;
		synchronized (monitor)
		{
			monitor.notify();
		}
	}

	final static boolean DEBUG = false;

	static void debug(String s)
	{
		if (DEBUG)
		{
			System.out.println(s);
		}
	}

	static void debug(Exception e)
	{
		if (DEBUG)
		{
			e.printStackTrace(System.out);
		}
	}

}

final class ServerThread extends Thread
{
	Server server;

	ServerSocket sv;

	ArrayList<Processor> ps = new ArrayList<Processor>();

	public ServerThread(Server server, boolean SSL)
	{
		try
		{
			this.server = server;
			if (SSL)
			{
				System.out.println("listening port:" + server.currentConfig.SSLport + "(SSL)");
				Log.log("listening port:" + server.currentConfig.SSLport + "(SSL)");
				sv = getSSLServerSocket(server.currentConfig);
			}
			else
			{
				System.out.println("listening port:" + server.currentConfig.port);
				Log.log("listening port:" + server.currentConfig.port);
				sv = new ServerSocket(server.currentConfig.port);
			}
		}
		catch (Exception e)
		{
			Log.log(e);
		}
	}

	@Override
	public void run()
	{
		try
		{
			while (true)
			{
				if (sv == null)
				{
					break;
				}
				try
				{
					Socket s;
					s = sv.accept();
					Processor p = new Processor(s, server, ps);
					String ip = ((InetSocketAddress) s.getRemoteSocketAddress()).getAddress().getHostAddress();
					String port = Integer.toString(((InetSocketAddress) s.getRemoteSocketAddress()).getPort());
					p.setName("Request Prosessor(" + ip + ":" + port + ")");
					ps.add(p);
					Log.log("connection at:" + sv.getLocalPort() + " ip:" + ip + " port:" + port);
					p.start();
				}
				catch (SocketException e)
				{
					if ("socket closed".equals(e.getMessage()))
					{
						break;
					}
				}
				catch (Exception e)
				{
					Log.log(e);
				}
			}
		}
		catch (Exception e)
		{
			Log.log(e);
		}
		finally
		{
			try
			{
				sv.close();
			}
			catch (Exception e)
			{
				//
			}
		}
	}

	public void shutdown()
	{
		try
		{
			if (sv != null)
			{
				sv.close();
			}
			sv = null;
		}
		catch (IOException e)
		{
			//
		}
		for (Processor p : ps)
		{
			p.shutdown();
		}
	}

	private static ServerSocket getSSLServerSocket(ServerConfig currentConfig) throws Exception
	{
		ServerSocket s = null;

		char keyStorePass[] = currentConfig.SSLkeyStorePass.toCharArray();
		char keyPassword[] = currentConfig.SSLkeyPassword.toCharArray();

		// KeyStore ks = KeyStore.getInstance("JKS");
		KeyStore ks = KeyStore.getInstance(KeyStore.getDefaultType());

		ks.load(new FileInputStream(currentConfig.SSLclientKeysFile), keyStorePass);

		// KeyManagerFactory kmf = KeyManagerFactory.getInstance("SunX509");
		KeyManagerFactory kmf = KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());

		kmf.init(ks, keyPassword);

		SSLContext sslContext = SSLContext.getInstance("SSLv3");
		// SSLContext sslContext = SSLContext.getInstance("TLS");
		// SSLContext sslContext = SSLContext.getInstance("SSL");
		// SSLContext sslContext = SSLContext.getInstance("TLSv1");

		sslContext.init(kmf.getKeyManagers(), null, null);

		SSLServerSocketFactory factory = sslContext.getServerSocketFactory();

		s = factory.createServerSocket(currentConfig.SSLport);

		return s;
	}
}

final class Monitor extends Thread
{
	Server server;

	boolean stop = false;

	Monitor(Server server)
	{
		this.server = server;
	}

	@Override
	public void run()
	{
		while (true)
		{

			server.st = new ServerThread(server, false);
			server.st.setName("Request Listener");
			server.st.start();
			if (server.currentConfig.SSLactive)
			{
				server.sst = new ServerThread(server, true);
				server.sst.setName("Request Listener(ssl)");
				server.sst.start();
			}

			if (server.st.sv == null)
			{
				break;
			}

			try
			{
				synchronized (this)
				{
					this.wait();
				}
			}
			catch (InterruptedException e)
			{
				//
			}
			try
			{
				synchronized (this)
				{
					this.wait(100);
				}
			}
			catch (InterruptedException e)
			{
				//
			}

			server.applyConfig();

			if (server.st != null)
			{
				server.st.shutdown();
			}
			if (server.currentConfig.SSLactive)
			{
				if (server.sst != null)
				{
					server.sst.shutdown();
				}
			}

			if (stop == true)
			{
				break;
			}
		}
		Log.close();
		server.sessionCleaner.cancel();
		// end;
	}
}

class ServletPack
{

	String url;

	Class<?> clazz;

	Servlet newInstance()
	{
		try
		{
			Servlet servlet = (Servlet) clazz.newInstance();
			servlet.url = url;
			return servlet;
		}
		catch (Exception e)
		{
			return null;
		}
	}
}
