package sample;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;

import webServer.Request;
import webServer.Response;
import webServer.Server;
import webServer.Servlet;

public class MapServlet extends Servlet {
	@Override
	public void doReq(Request req, Response res, Server server) throws Exception {
		int w = 804;
		int h = 804;
		String player = (String) req.getSession().get("player");
		String group = (String) req.getSession().get("group");
		int[][] m = new int[w][h];

		try {
			FileInputStream fis = new FileInputStream("map.sql");
			InputStreamReader isr = new InputStreamReader(fis, "utf-8");
			BufferedReader br = new BufferedReader(isr);

			while (true) {
				String s = br.readLine();
				if (s == null)
					break;
				s = s.substring(30, s.length() - 2);

				char[] c = s.toCharArray();
				int sts = -1;
				for (int i = 0; i < c.length; i++) {
					if (c[i] == '\'') {
						sts = -sts;
					}
					if (sts == 1 && c[i] == ',')
						c[i] = '、';
				}
				s = new String(c);
				String[] s3 = s.split(",");
				int x = 0;
				int y = 0;
				int p = 0;
				try {
					x = Integer.parseInt(s3[1]);
					y = Integer.parseInt(s3[2]);
					p = Integer.parseInt(s3[10]);
				} catch (Exception e) {
					System.out.print(e + "\n");
					e.printStackTrace();
				}
				if (s3.length != 11)
					System.out.print(s3.length);
				int v;
				if (player != null && ("'" + player + "'").equals(s3[7])) {
					v = 255 * 256 * 256;
				} else if (group != null && !"".equals(group) && ("'" + group + "'").equals(s3[9])) {
					v = 255 * 256 * 256;
				} else {
					v = (255 - top(p * 255 / 1000)) + top(p * 255 / 1000) * 256;
				}
				m[x + 402][y + 402] = v;
			}
		} catch (Exception e) {
			System.out.print(e + "\n");
			e.printStackTrace();
		}

		int[] a = new int[w * h];

		int idx = 0;
		for (int i = 0; i < w; i++) {
			for (int j = 0; j < h; j++) {
				a[idx] = m[i][j];

				j -= 402;
				i -= 402;
				if (i == 12 && j == -18 || i == 46 && j == 36 || i == 143 && j == 0 || i == 138 && j == 164 || i == 36 && j == -48 || i == -15 && j == -141 || i == 162 && j == -141 || i == -47 && j == -38 || i == -144 && j == 15 || i == -169 && j == -132 || i == -46 && j == 37 || i == 8 && j == 144 || i == -133 && j == 165) {
					a[idx] = 0x00FF0000;
				}
				j += 402;
				i += 402;
				idx++;
			}
		}
		byte[] b = getByte(a, w, h);

		res.setFileName("a.bmp");

		OutputStream out = res.getOutputStream();
		out.write(b);
	}

	private int top(int a) {
		if (a > 255)
			return 255;
		else
			return a;
	}

	public static byte[] getByte(int[] value, int w, int h) {
		try {
			byte[] head = { 0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0, 0x36, 0, 0, 0 }; //BM / file end / cltb end
			byte[] info = { 0x28, 0, 0, 0, //bi.biSize = &H28
					((byte) (w & 0xFF)), ((byte) (w >> 8)), 0, 0, //bi.biWidth = Width
					((byte) (h & 0xFF)), ((byte) (h >> 8)), 0, 0, //bi.biHeight = Height
					0x01, 0, 0x18, 0, //bi.biPlanes = 1 bi.biBitCount = 24
					0, 0, 0, 0, //bi.biCompression = 0'BI_RGB
					0, 0, 0, 0, //bi.biSizeImage = 0
					0, 0, 0, 0, //bi.biXPelsPerMeter = 0
					0, 0, 0, 0, //bi.biYPelsPerMeter = 0
					0, 0, 0, 0, //bi.biClrUsed = 0
					0, 0, 0, 0, //bi.biClrImportant = 0
			};
			byte bits[] = new byte[w * h * 3];
			int k = 0;
			for (int i = h - 1; i >= 0; i--) {
				for (int j = 0; j < w; j++) {
					int v = value[k];
					bits[(j + i * w) * 3] = (byte) (v & 0xFF);
					bits[(j + i * w) * 3 + 1] = (byte) ((v >> 8) & 0xFF);
					bits[(j + i * w) * 3 + 2] = (byte) ((v >> 16) & 0xFF);
					k++;
				}
			}
			byte[] binout = new byte[bits.length + head.length + info.length];
			int idx = 0;
			for (int i = 0; i < head.length; i++) {
				binout[idx++] = head[i];
			}
			for (int i = 0; i < info.length; i++) {
				binout[idx++] = info[i];
			}
			for (int i = 0; i < bits.length; i++) {
				binout[idx++] = bits[i];
			}
			return binout;
		} catch (Exception e) {
			System.out.print(e);
			return null;
		}
	}
}