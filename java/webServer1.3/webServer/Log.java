package webServer;

import java.io.FileOutputStream;
import java.io.PrintStream;

public class Log
{
	private static PrintStream log = System.out;

	public static void initLog(String logInFile)
	{
		try
		{
			if (logInFile != null && logInFile != "")
			{
				FileOutputStream fos = new FileOutputStream(logInFile, true);
				log = new PrintStream(fos, true, "utf-8");
			}
			else
			{
				log = System.out;
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}

	}

	static void log(Exception e)
	{
		if (Log.log != null)
		{
			log.println(Util.getTime() + ":");
			e.printStackTrace(log);
			log.println();
			log.flush();
		}
	}

	static void log(String s)
	{
		if (Log.log != null)
		{
			log.print(Util.getTime() + ":");
			log.println(s);
			log.flush();
		}
	}

	static void close()
	{
		if (Log.log != null)
		{
			log.close();
		}
	}
}
