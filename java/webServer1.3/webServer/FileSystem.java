package webServer;

import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

import javax.imageio.ImageIO;
import javax.imageio.ImageReadParam;
import javax.imageio.ImageReader;
import javax.imageio.stream.ImageInputStream;

import com.sun.image.codec.jpeg.JPEGCodec;
import com.sun.image.codec.jpeg.JPEGEncodeParam;
import com.sun.image.codec.jpeg.JPEGImageEncoder;

final class FileSystem
{
	private String url = null;

	private String rootPath = null;

	private boolean allowList = false;

	private boolean allowUpLoad = false;

	private boolean allowDel = false;

	private boolean isDownload = true;

	private String userName = null;

	private String passWord = null;

	FileSystem(String url, String rootPath, boolean allowList, boolean allowUpLoad, boolean allowDel, boolean isDownload, String userName, String passWord)
	{
		this.url = url;

		if (rootPath.endsWith(Server.SEP))
		{
			this.rootPath = rootPath;
		}
		else
		{
			this.rootPath = rootPath + Server.SEP;
		}

		this.allowList = allowList;

		this.allowUpLoad = allowUpLoad;

		this.allowDel = allowDel;

		this.isDownload = isDownload;

		this.userName = userName;

		this.passWord = passWord;
	}

	String getUrl()
	{
		return url;
	}

	boolean authorization(String userName, String passWord)
	{
		String u1, u2, p1, p2;

		if (userName == null)
		{
			u1 = "";
		}
		else
		{
			u1 = userName;
		}
		if (this.userName == null)
		{
			u2 = "";
		}
		else
		{
			u2 = this.userName;
		}

		if (passWord == null)
		{
			p1 = "";
		}
		else
		{
			p1 = passWord;
		}
		if (this.passWord == null)
		{
			p2 = "";
		}
		else
		{
			p2 = this.passWord;
		}

		if (u2 == "" && p2 == "")
		{
			return true;
		}

		if (u1.equals(u2) && p2.equals(Util.MD5(p1)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void doReq(String path, Request req, Response res, Server server) throws Exception
	{
		Session session = req.getSession();

		String action = req.getPOSTParam("action");

		if (userName != null || passWord != null)
		{
			if ("login".equals(action))
			{
				boolean pass = authorization(req.getPOSTParam("username"), req.getPOSTParam("password"));

				if (pass)
				{
					session.setPrivate("FileLogin" + this.url, "T");
				}
				else
				{
					return;
				}
			}
			else if ("logout".equals(action))
			{
				session.setPrivate("FileLogin" + this.url, null);
				makeLogin(req, res);
				return;
			}
			else
			{
				if (!"T".equals(session.getPrivate("FileLogin" + this.url)))
				{
					makeLogin(req, res);
					return;
				}
			}
		}

		String fn = path.substring(url.length());
		fn = rootPath + fn.replace("/", Server.SEP);

		fn = fn.replace("..", "");

		File file = new File(fn);

		if (file.isDirectory())
		{
			String del = req.getGETParam("del");
			String sort = req.getGETParam("sort");

			if (sort != null)
			{
				String sort1 = (String) session.getPrivate("sort1");
				String sort2 = (String) session.getPrivate("sort2");
				if ("none".equals(sort))
				{
					sort1 = null;
					sort2 = null;
				}
				else
				{
					sort2 = sort1;
					sort1 = sort;
				}
				session.setPrivate("sort1", sort1);
				session.setPrivate("sort2", sort2);

				res.sendRedirect(".");
				return;
			}
			else if (allowDel && del != null)
			{
				File delFile = new File(file.getPath() + Server.SEP + del);
				delFile.delete();
				res.sendRedirect(".");
				return;
			}
			else if (allowUpLoad && req.getMultipartData("file") != null)
			{
				File upFile = new File(req.getMultipartData("file").filename);
				try
				{
					String upPath = file.getPath() + Server.SEP + upFile.getName();
					FileOutputStream fos = new FileOutputStream(upPath);
					fos.write(req.getMultipartData("file").bArray);
					fos.close();
				}
				catch (Exception e)
				{
					Log.log(e);
				}
				res.sendRedirect(".");
				return;
			}
			else
			{
				getList(file, req, res);
				return;
			}
		}
		else
		{
			String preView = req.getGETParam("prev");

			if (isImg(file.getName()) && preView != null)
			{
				if ("thumbnail".equals(preView))
				{
					res.setFileName(file.getName());
					OutputStream out = res.getOutputStream();
					makeThumbNail(160, 120, 0.75f, file, out);
					return;
				}
			}
			else
			{
				if (isDownload)
				{
					int start;
					int end;

					String Range = req.getHTTPParam("Range");

					start = 0;
					end = (int) file.length() - 1;

					if (Range != null)
					{
						Range = Range.substring(6);
						String[] Ranges = Range.split(",");
						if (Ranges.length > 0)
						{
							Ranges = Ranges[0].split("-");
							if (Ranges.length > 0)
							{
								start = Util.strToInt(Ranges[0]);
							}
							if (Ranges.length > 1)
							{
								end = Util.strToInt(Ranges[1]);
							}
						}
					}

					setFileHead(file, start, end, res);
				}
				else
				{
					FileInputStream fis = new FileInputStream(file);
					InputStreamReader isr = new InputStreamReader(fis, server.currentConfig.getDefaultEncoding());
					try
					{
						PrintWriter pw = res.getWriter();
						char[] cbuff = new char[1024];
						while (true)
						{
							int len = isr.read(cbuff);
							if (len <= 0) break;
							pw.write(cbuff, 0, len);
						}
					}
					finally
					{
						isr.close();
						fis.close();
					}
				}
			}
		}
	}

	private static void makeThumbNail(int wid, int hei, float q, File fis, OutputStream os) throws Exception
	{

		BufferedImage b = null;
		ImageInputStream stream = null;

		try
		{
			stream = ImageIO.createImageInputStream(fis);
			Iterator<ImageReader> iter = ImageIO.getImageReaders(stream);
			ImageReader reader = iter.next();
			reader.setInput(stream, true, true);

			float origRate = ((float) reader.getWidth(0)) / reader.getHeight(0);

			float rate = (float) wid / (float) hei;

			int targetW, targetH;
			if (rate > origRate)
			{
				targetW = (int) (hei * origRate);
				targetH = hei;
			}
			else
			{
				targetW = wid;
				targetH = (int) (wid / origRate);
			}

			ImageReadParam param = new ImageReadParam();
			int w = reader.getWidth(0) / targetW;
			int h = reader.getHeight(0) / targetH;
			if (w <= 0) w = 1;
			if (h <= 0) h = 1;

			param.setSourceSubsampling(w, h, 0, 0);
			BufferedImage bi = reader.read(0, param);

			b = new BufferedImage(targetW, targetH, BufferedImage.TYPE_INT_RGB);
			Graphics2D g = b.createGraphics();

			g.drawImage(bi, 0, 0, targetW, targetH, null);
		}
		catch (Throwable t)
		{
			//
		}
		finally
		{
			try
			{
				stream.close();
			}
			catch (Exception e)
			{
				//
			}
		}
		try
		{
			JPEGImageEncoder encoder = JPEGCodec.createJPEGEncoder(os);
			JPEGEncodeParam param = JPEGCodec.getDefaultJPEGEncodeParam(b);
			param.setQuality(q, false);
			encoder.setJPEGEncodeParam(param);
			encoder.encode(b);
		}
		catch (Throwable t)
		{
			//
		}
		finally
		{
			//
		}
	}

	static class Sorter implements Comparator<File>
	{
		int sort1 = 0;

		int sort2 = 0;

		public int compare(File f1, File f2)
		{
			if (f1.isDirectory() && !f2.isDirectory())
			{
				return -1;
			}
			if (!f1.isDirectory() && f2.isDirectory())
			{
				return 1;
			}
			String name1 = f1.getName().toLowerCase();
			String name2 = f2.getName().toLowerCase();
			Long size1 = new Long(f1.length());
			Long size2 = new Long(f2.length());
			int v;

			if (sort1 == 1)
			{
				v = name1.compareTo(name2);
				if (v != 0)
				{
					return v;
				}
				if (sort2 == 2)
				{
					v = size1.compareTo(size2);
					return v;
				}
			}
			else if (sort1 == 2)
			{
				v = size1.compareTo(size2);
				if (v != 0)
				{
					return v;
				}
				if (sort2 == 1)
				{
					v = name1.compareTo(name2);
					return v;
				}
			}
			return 0;
		}
	}

	private static void makeLogin(Request req, Response res) throws Exception
	{
		PrintWriter out = res.getWriter();
		out.println("<html>");
		out.println("<body>");
		out.println("<form name = 'formMain' method = 'post'>");
		out.println("<input type = \"hidden\" name = \"action\" id = \"action\" value=\"login\">");
		out.println("username:<input type = \"text\" name = \"username\"  id = \"username\"><br>");
		out.println("password:<input type = \"password\" name = \"password\" id = \"password\"><br>");
		out.println("<input type = \"submit\"><br>");
		out.println("</form>");
		out.println("</body>");
		out.println("</html>");
	}

	private void getList(File file, Request req, Response res) throws Exception
	{
		Session session = req.getSession();

		Sorter s = new Sorter();

		List<File> files = new ArrayList<File>();
		Collections.addAll(files, file.listFiles());
		DecimalFormat fmt = new DecimalFormat("###,###");
		long total = 0;
		String sort1 = (String) session.getPrivate("sort1");
		String sort2 = (String) session.getPrivate("sort2");

		if ("name".equals(sort1))
		{
			s.sort1 = 1;
		}
		if ("size".equals(sort1))
		{
			s.sort1 = 2;
		}
		if ("name".equals(sort2))
		{
			s.sort2 = 1;
		}
		if ("size".equals(sort2))
		{
			s.sort2 = 2;
		}
		if (s.sort1 != 0 || s.sort2 != 0)
		{
			Collections.<File> sort(files, s);
		}

		PrintWriter out = res.getWriter();

		out.println("<html>");
		out.println("<body>");
		out.println("<table border = 1>");

		out.println("<tr>");
		out.println("	<td>");
		out.println(" <a href='./?sort=name'>filename</a> <a href='./?sort=none'>cancel sort</a>");
		out.println("	</td>");
		out.println("	<td align = 'right'>");
		out.println("	<a href='./?sort=size'>size</a>");
		out.println("	</td>");
		if (allowDel)
		{
			out.println("	<td>&nbsp;</td>");
		}
		out.println("</tr>");

		if (!this.url.equals(req.getPath()))
		{
			out.println("<tr>");
			out.println("	<td>");
			out.println("		<a href='../'>../</a>");
			out.println("	</td>");
			out.println("	<td align = 'right'>&nbsp;</td>");
			if (allowDel)
			{
				out.println("	<td>&nbsp;</td>");
			}
			out.println("</tr>");
		}

		for (File f : files)
		{
			out.println("<tr>");
			out.println("	<td>");
			String fileName;
			if (f.isDirectory())
			{
				fileName = f.getName() + "/";
			}
			else
			{
				fileName = f.getName();
			}
			if (isImg(fileName))
			{
				out.println("		<img src = '" + fileName + "?prev=thumbnail'>");
			}
			out.println("		<a href=\"" + fileName + "\">" + fileName + "</a>");
			out.println("	</td>");
			out.println("	<td align = 'right'>");
			out.println("		" + fmt.format(f.length()) + "");
			total += f.length();
			out.println("	</td>");
			if (allowDel)
			{
				if (f.isDirectory())
				{
					out.println("	<td>&nbsp;</td>");
				}
				else
				{
					out.println("	<td>");
					out.println("		<a href=\"./?del=" + fileName + "\">del</a>");
					out.println("	</td>");
				}
			}
			out.println("</tr>");
		}
		out.println("<tr>");
		out.println("	<td>total:</td>");
		out.println("	<td align = 'right'>" + fmt.format(total) + "</td>");
		if (allowDel)
		{
			out.println("	<td>&nbsp;</td>");
		}

		out.println("</tr>");

		if (allowUpLoad)
		{
			out.println("<form action = './' method = 'post' enctype='multipart/form-data'>");
			out.println("<tr>");
			out.println("	<td>");
			out.println("		upload: <input type = file name = 'file'>");
			out.println("	</td>");
			out.println("	<td align = 'right'><input type = submit value = upload></td>");
			if (allowDel)
			{
				out.println("	<td>&nbsp;</td>");
			}
			out.println("</tr>");
			out.println("</form>");
		}

		out.println("</table>");
		out.println("</body>");
		out.println("</html>");
	}

	private void setFileHead(File file, int start, int end, Response res) throws IOException
	{
		FileInputStream fis = new FileInputStream(file);

		if (start == 0 && end == file.length() - 1)
		{
			res.setHeader("Accept-Ranges", "bytes");
			res.setHeader("ETag", "\"\"");
			Calendar c = Calendar.getInstance();
			c.setTimeInMillis(file.lastModified());
			res.setHeader("Last-Modified", Util.getGMTString(c));

			res.setInputStream(fis, (int) file.length());
		}
		else
		{
			res.head = Response.HEAD206;
			res.setHeader("Accept-Ranges", "bytes");
			res.setHeader("ETag", "\"\"");
			res.setHeader("Content-Range", "bytes " + start + "-" + end + "/" + file.length());

			fis.skip(start);

			res.setInputStream(fis, end - start + 1);
		}
	}

	boolean isExist(String req, Server server)
	{
		String fn = req.substring(url.length());

		File file = new File(rootPath + fn.replace("/", Server.SEP));

		if (file.exists())
		{
			if (file.isDirectory())
			{
				if (allowList)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	private boolean isImg(String filename)
	{
		filename = filename.toLowerCase();
		return (filename.endsWith(".jpeg") || filename.endsWith(".jpg") || filename.endsWith(".gif") || filename.endsWith(".bmp"));
	}
}
