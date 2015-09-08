import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

public class SatelliteMTSAT2R
{
	public static void main(String[] argv) throws Exception
	{
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");

		SimpleDateFormat sdf2 = new SimpleDateFormat("yyyy/MMM", Locale.ENGLISH);
		SimpleDateFormat sdf3 = new SimpleDateFormat("yyyyMMddHH", Locale.ENGLISH);
//MTSAT2R201209172332IR1.jpg
		Calendar c = Calendar.getInstance();
		Date d = sdf.parse("2012/12/01 00:00:00");
		c.setTime(d);
		Date end = sdf.parse("2013/01/01 00:00:00");
		Calendar cend = Calendar.getInstance();
		cend.setTime(end);
		while (true)
		{
			if (c.after(cend))
			{
				break;
			}

			Date d2 = c.getTime();
			//String date = "MTSAT2R" + sdf3.format(d2);
			String date = "MTSAT1R" + sdf3.format(d2);

			//download(date + "32IR1.jpg", sdf2, d2);
			//download(date + "32IR3.jpg", sdf2, d2);
			//download(date + "32VIS.jpg", sdf2, d2);
			download(date + "30IR1.jpg", sdf2, d2);
			download(date + "30IR3.jpg", sdf2, d2);
			download(date + "30VIS.jpg", sdf2, d2);

			c.add(Calendar.HOUR, 1);

		}
	}
	private static String path = "F:\\mtsat\\";
	
	private static void download(String fn, SimpleDateFormat sdf2, Date d) throws Exception
	{
			String su = "http://webgms.iis.u-tokyo.ac.jp/IIS/L1B/" + sdf2.format(d) + "/img/" + fn;
		try
		{
			byte[] buff = new byte[1024 * 1024 * 1];

			URL u = new URL(su);
			InputStream is = u.openStream();
			FileOutputStream fos = new FileOutputStream(path + fn);

			while (true)
			{
				int len = is.read(buff);
				if (len < 0)
				{
					break;
				}
				fos.write(buff, 0, len);
			}
			is.close();

			fos.close();
		}
		catch (Exception e)
		{
			System.out.println(su);
			FileOutputStream fos = new FileOutputStream(path + fn);
			fos.close();
		}
	}
}
