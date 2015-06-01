package sample;

import java.awt.Dimension;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.Toolkit;
import java.awt.image.BufferedImage;
import java.io.OutputStream;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;
import webServer.Session;

import com.sun.image.codec.jpeg.JPEGCodec;
import com.sun.image.codec.jpeg.JPEGEncodeParam;
import com.sun.image.codec.jpeg.JPEGImageEncoder;

public class ScreenServlet extends Servlet
{
	@Override
	public void doReq(Request req, Response res, Server server) throws Exception
	{

		Session session = req.getSession();
		int iquality = strToInt((String) session.get("quality"));
		if (!"T".equals(session.get(LoginServlet.LOGIN))) {
			res.sendRedirect("Login");
			return;
		}
		if (iquality < 100)
		{
			res.setGZIP(false);
			
			//res.setFileName("screen.jpg");
			
			float fquality;
			if (iquality > 0)
			{
				fquality = (iquality) / 100f;
			}
			else
			{
				fquality = 0.5f;
			}
			BufferedImage b = captureScreen();
			OutputStream out = res.getOutputStream();
			
			res.setContentType("image/jpeg");

			JPEGImageEncoder encoder = JPEGCodec.createJPEGEncoder(out);
			try
			{
				JPEGEncodeParam param = JPEGCodec.getDefaultJPEGEncodeParam(b);
				param.setQuality(fquality, false);
				encoder.setJPEGEncodeParam(param);
				encoder.encode(b);
			}
			catch (Exception e)
			{
				System.out.print(e);
			}
		}
		if (iquality == 100)
		{
			res.setFileName("screen.bmp");
			BufferedImage b = captureScreen();
			Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
			int[] i = b.getRGB(0, 0, scrSize.width, scrSize.height, null, 0, scrSize.width);
			OutputStream out = res.getOutputStream();
			out.write(getByte(i, scrSize.width, scrSize.height));
		}
	}

	private BufferedImage captureScreen()
	{
		try
		{
			Robot robot = new Robot();
			Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
			BufferedImage bufImg = robot.createScreenCapture(new Rectangle(0, 0, scrSize.width, scrSize.height));
			return bufImg;
		}
		catch (Exception e)
		{
			System.out.println(e.toString());
			return null;
		}
	}

	public static byte[] getByte(int[] value, int w, int h)
	{
		try
		{
			byte[] head = { 0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0, 0x36, 0, 0, 0 }; // BM /
			// file
			// end
			// /
			// cltb
			// end
			byte[] info = { 0x28, 0, 0, 0, // bi.biSize = &H28
					((byte) (w & 0xFF)), ((byte) (w >> 8)), 0, 0, // bi.biWidth
					// = Width
					((byte) (h & 0xFF)), ((byte) (h >> 8)), 0, 0, // bi.biHeight
					// = Height
					0x01, 0, 0x18, 0, // bi.biPlanes = 1 bi.biBitCount = 24
					0, 0, 0, 0, // bi.biCompression = 0'BI_RGB
					0, 0, 0, 0, // bi.biSizeImage = 0
					0, 0, 0, 0, // bi.biXPelsPerMeter = 0
					0, 0, 0, 0, // bi.biYPelsPerMeter = 0
					0, 0, 0, 0, // bi.biClrUsed = 0
					0, 0, 0, 0, // bi.biClrImportant = 0
			};
			byte bits[] = new byte[w * h * 3];
			int k = 0;
			for (int i = h - 1; i >= 0; i--)
			{
				for (int j = 0; j < w; j++)
				{
					int v = value[k];
					bits[(j + i * w) * 3] = (byte) (v & 0xFF);
					bits[(j + i * w) * 3 + 1] = (byte) ((v >> 8) & 0xFF);
					bits[(j + i * w) * 3 + 2] = (byte) ((v >> 16) & 0xFF);
					k++;
				}
			}
			byte[] binout = new byte[bits.length + head.length + info.length];
			int idx = 0;
			for (int i = 0; i < head.length; i++)
			{
				binout[idx++] = head[i];
			}
			for (int i = 0; i < info.length; i++)
			{
				binout[idx++] = info[i];
			}
			for (int i = 0; i < bits.length; i++)
			{
				binout[idx++] = bits[i];
			}
			return binout;
		}
		catch (Exception e)
		{
			System.out.print(e);
			return null;
		}
	}

	private int strToInt(String sNum)
	{
		int iNum = 0;
		try
		{
			iNum = Integer.parseInt(sNum);
		}
		catch (Exception e)
		{
			iNum = 0;
		}
		return iNum;
	}
}