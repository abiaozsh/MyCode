package com.wch.Radar;

import java.io.ByteArrayInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;
import java.text.NumberFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

public class NetThread extends Thread {

	static NumberFormat nf = NumberFormat.getInstance();

	static NumberFormat nf4 = NumberFormat.getInstance();
	static {
		nf.setGroupingUsed(false);
		nf.setMinimumIntegerDigits(2);
		nf4.setGroupingUsed(false);
		nf4.setMinimumIntegerDigits(4);
	}

	/* constructor */
	Handler mhandler;
	ImgContainer imgContainer;
	String cmd;
	Activity act;

	NetThread(String cmd, Handler h, ImgContainer imgContainer, Activity act) {
		mhandler = h;
		this.imgContainer = imgContainer;
		this.cmd = cmd;
		this.act = act;
		this.setPriority(Thread.MIN_PRIORITY);
	}

	public void run() {
		if ("test".equals(cmd)) {
			Calendar cal = Calendar.getInstance();
			cal.setTime(new Date());
			cal.add(Calendar.MINUTE, -20);

			imgContainer.bmp = getData(cal);
			sendCmd("updateimg");

		} else if ("clearall".equals(cmd)) {

			String[] fl = act.fileList();
			for (int i = 0; i < fl.length; i++) {
				act.deleteFile(fl[i]);
			}

		} else if ("ld1h".equals(cmd)) {
			play(1, 10, 300);
		} else if ("ld2h".equals(cmd)) {
			play(2, 20, 200);
		} else if ("ld1d".equals(cmd)) {
			play(24, 240, 50);
		} else if ("ld2d".equals(cmd)) {
			play(48, 480, 50);
		}
	}

	void sendCmd(String cmd) {
		Message m = mhandler.obtainMessage();
		Bundle b = new Bundle();
		b.putString("cmd", cmd);
		m.setData(b);
		mhandler.sendMessage(m);
	}

	void sendMsg(String msg) {
		Message m = mhandler.obtainMessage();
		Bundle b = new Bundle();
		b.putString("msg", msg);
		m.setData(b);
		mhandler.sendMessage(m);
	}

	public void play(int hours, int cnt, int sleep) {
		SimpleDateFormat fmtYMDHMSS = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

		Calendar cal = Calendar.getInstance();
		cal.add(Calendar.HOUR, -hours);

		for (int i = 0; i < cnt; i++) {
			String date = fmtYMDHMSS.format(cal.getTime());
			System.out.println(date);

			imgContainer.bmp = getData(cal);
			sendCmd("updateimg");

			cal.add(Calendar.MINUTE, 6);

			try {
				Thread.sleep(sleep);// ºÁÃë
			} catch (Exception e) {
			}
		}

	}

	public Bitmap getData(Calendar cal) {
		try {
			sendMsg("start");
			String[] fl = act.fileList();
			String url = getUrl(cal);
			String name = getFilename(cal);

			boolean found = false;
			for (int i = 0; i < fl.length; i++) {
				if (fl[i].equals(name)) {
					found = true;
					break;
				}
			}

			if (found) {
				InputStream _is = act.openFileInput(name);
				Bitmap bitmap = BitmapFactory.decodeStream(_is);
				_is.close();
				return bitmap;
			} else {

				URL realUrl = new URL(url);
				URLConnection conn = realUrl.openConnection();
				conn.setRequestProperty("accept", "*/*");
				// conn.setRequestProperty("connection", "Keep-Alive");
				// conn.setRequestProperty("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;SV1)");
				conn.setRequestProperty("User-Agent", "Mozilla/4.0 (compatible; MSIE 5.0; Windows NT; DigExt)");
				// conn.setDoOutput(true);
				conn.setDoInput(true);
				// conn.connect();
				InputStream _is = conn.getInputStream();
				byte[] buff = new byte[1024 * 1024 * 10];
				int idx = 0;
				while (true) {
					int data = _is.read();
					if (data == -1) {
						break;
					}
					buff[idx++] = (byte) data;
				}
				_is.close();
				if (idx > 100000) {
					ByteArrayInputStream bais = new ByteArrayInputStream(buff, 0, idx);
					Bitmap bitmap = BitmapFactory.decodeStream(bais);
					FileOutputStream fos = act.openFileOutput(name, Activity.MODE_APPEND);
					fos.write(buff, 0, idx);
					fos.close();
					return bitmap;
				}
				return null;
			}

		} catch (FileNotFoundException e) {
			sendMsg("notfound");
		} catch (Exception e) {
		}
		return null;
	}

	public static String getFilename(Calendar c) {
		String year = nf4.format(c.get(Calendar.YEAR));
		String month = nf.format(c.get(Calendar.MONTH) + 1);
		String day = nf.format(c.get(Calendar.DATE));

		String hour = nf.format(c.get(Calendar.HOUR_OF_DAY));

		int minute = c.get(Calendar.MINUTE);
		for (int i = 60; i >= 0; i -= 6) {
			if (minute >= i) {
				minute = i;
				break;
			}
		}

		String min = nf.format(minute);

		return year + month + day + hour + min + ".PNG";
	}

	public static String getUrl(Calendar cal) {
		Calendar c = Calendar.getInstance();
		c.setTime(cal.getTime());
		c.add(Calendar.HOUR, -8);

		String y = nf4.format(c.get(Calendar.YEAR));
		String m = nf.format(c.get(Calendar.MONTH) + 1);
		String d = nf.format(c.get(Calendar.DATE));

		String hour = nf.format(c.get(Calendar.HOUR_OF_DAY));

		int minute = c.get(Calendar.MINUTE);
		for (int i = 60; i >= 0; i -= 6) {
			if (minute >= i) {
				minute = i;
				break;
			}
		}

		String min = nf.format(minute);

		String fy = nf4.format(cal.get(Calendar.YEAR));
		String fm = nf.format(cal.get(Calendar.MONTH) + 1);
		String fd = nf.format(cal.get(Calendar.DATE));

		return "http://image.nmc.cn/product/" + fy + "/" + fm + "/" + fd + "/RDCP/medium/SEVP_AOC_RDCP_SLDAS_EBREF_AZ9002_L88_PI_" + y + m + d + hour + min + "00000.PNG";
	}

}
