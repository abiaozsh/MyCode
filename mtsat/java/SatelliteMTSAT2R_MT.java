import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class SatelliteMTSAT2R {

	public static void main(String[] argv) throws Exception {
		// "http://webgms.iis.u-tokyo.ac.jp/IIS/L1B/"
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");

		SimpleDateFormat sdf2 = new SimpleDateFormat("yyyy/MMM", Locale.ENGLISH);
		SimpleDateFormat sdf3 = new SimpleDateFormat("yyyyMMddHH", Locale.ENGLISH);

		Calendar c = Calendar.getInstance();
		Date d = sdf.parse("2012/08/27 03:00:00");
		// Date d = sdf.parse("2011/11/10 02:00:00");
		c.setTime(d);
		Date end = sdf.parse("2014/08/10 00:00:00");
		// Date end = sdf.parse("2011/12/26 04:00:00");
		Calendar cend = Calendar.getInstance();
		cend.setTime(end);
		while (true) {
			if (c.after(cend)) {
				break;
			}

			Date d2 = c.getTime();
			String min = "32";
			String date = "MTSAT2R" + sdf3.format(d2) + min;
			String date2 = sdf3.format(d2) + min;

			String minback = "30";
			String dateback = "MTSAT1R" + sdf3.format(d2) + minback;
			String date2back = sdf3.format(d2) + minback;

			CountDownLatch cnt = new CountDownLatch(3);

			Download download1 = new Download(date + "IR1.jpg", date2 + "IR1.jpg", dateback + "IR1.jpg", date2back + "IR1.jpg", sdf2, d2, cnt);
			// download(date + "30IR2.jpg", sdf2, d2);
			Download download2 = new Download(date + "IR3.jpg", date2 + "IR3.jpg", dateback + "IR3.jpg", date2back + "IR3.jpg", sdf2, d2, cnt);
			// download(date + "30IR4.jpg", sdf2, d2);
			Download download3 = new Download(date + "VIS.jpg", date2 + "VIS.jpg", dateback + "VIS.jpg", date2back + "VIS.jpg", sdf2, d2, cnt);

			download1.start();
			download2.start();
			download3.start();

			cnt.await(60, TimeUnit.SECONDS);

			c.add(Calendar.HOUR, 1);

		}
	}

}

class Download extends Thread {

	String fn;
	String fn2;
	String fnback;
	String fn2back;
	SimpleDateFormat sdf2;
	Date d;
	CountDownLatch cnt;

	public Download(String fn, String fn2, String fnback, String fn2back, SimpleDateFormat sdf2, Date d, CountDownLatch cnt) {
		this.fn = fn;
		this.fn2 = fn2;
		this.fnback = fnback;
		this.fn2back = fn2back;
		this.sdf2 = sdf2;
		this.d = d;
		this.cnt = cnt;
	}

	public void run() {
		try {
			try {
				String su = "http://webgms.iis.u-tokyo.ac.jp/IIS/L1B/" + sdf2.format(d) + "/img/" + fn;
				byte[] buff = new byte[1024 * 1024 * 1];

				URL u = new URL(su);

				InputStream is = u.openStream();

				FileOutputStream fos = new FileOutputStream("E:\\mtsat\\" + fn2);

				while (true) {
					int len = is.read(buff);
					if (len < 0) {
						break;
					}
					fos.write(buff, 0, len);
				}
				is.close();

				fos.close();
			} catch (Exception e) {
				try {
					String su = "http://webgms.iis.u-tokyo.ac.jp/IIS/L1B/" + sdf2.format(d) + "/img/" + fnback;
					byte[] buff = new byte[1024 * 1024 * 1];

					URL u = new URL(su);

					InputStream is = u.openStream();

					FileOutputStream fos = new FileOutputStream("E:\\mtsat\\" + fn2back);

					while (true) {
						int len = is.read(buff);
						if (len < 0) {
							break;
						}
						fos.write(buff, 0, len);
					}
					is.close();

					fos.close();
				} catch (Exception ex) {
					System.out.println(fn);
					FileOutputStream fos = new FileOutputStream("E:\\mtsat\\" + fn2back);
					fos.close();
				}
			}
		} catch (Exception ex) {
		}
		cnt.countDown();
	}
}