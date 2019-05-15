using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.IO;
using System.Text;
using System.Threading.Tasks;

namespace LogTool
{
	static class Program
	{
		public static string root = "e:\\temp\\nude\\";

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			for (int i = 0; i <= 60; i += 20)
			{
				string url = "http://www.nudecollect.com/artistic_photos_1/index/category/4642-mplstudios_aida_good_time_girl_x71/start-" + i;
				proc(url, "4642-mplstudios_aida_good_time_girl_x71", i);
			}

			Console.WriteLine("finish");
			Console.ReadLine();
		}

		class Obj
		{
			public string url;
			public string dir;
			public string file;
		}

		public static void proc(string url, string name, int i)
		{
			string s = Request(url);
			int idx;
			string val;

			List<Obj> list = new List<Obj>();

			while (true)
			{
				idx = s.IndexOf("<div class=\"col-outer col-lg-3 col-md-4 col-sm-6 col-xs-12\" data-grid-classes=\"col-lg-3 col-md-4 col-sm-6 col-xs-12\">");
				if (idx < 0) break;
				s = s.Substring(idx);
				idx = s.IndexOf("../../_data/i/");
				if (idx < 0)
				{
					idx = s.IndexOf("../../../i/");
					s = s.Substring(idx);
					idx = s.IndexOf("\"");
					val = s.Substring(0, idx);
					url = val.Replace("../../../i/", "http://www.nudecollect.com/artistic_photos_1/");
				}
				else
				{
					s = s.Substring(idx);
					idx = s.IndexOf("\"");
					val = s.Substring(0, idx);
					url = val.Replace("../../_data/i/", "http://www.nudecollect.com/artistic_photos_1/");
				}
				url = url.Replace("-cu_e260x350", "");

				Obj o = new Obj();

				o.url = url;
				o.dir = root + name + "\\";
				o.file = (i++) + ".jpeg";
				list.Add(o);
			}
			Parallel.ForEach(list, o =>
			{
				RequestBin(o.url, o.dir, o.file);
				Console.WriteLine("done:" + o.file);
			});

			Console.WriteLine(i);
		}

		//public static void lv1()
		//{
		//
		//    int block = 0;
		//
		//    string file = @"e:\temp\nude\1\" + block + ".txt";
		//
		//    FileStream fs = new FileStream(file, FileMode.Open, FileAccess.Read);
		//    StreamReader sr = new StreamReader(fs, Encoding.GetEncoding("utf-8"));
		//
		//    string s = sr.ReadToEnd();
		//    fs.Close();
		//
		//    int n = 0;
		//    while (true)
		//    {
		//        int idx = s.IndexOf("<div class=\"col-outer col-lg-3 col-md-4 col-sm-6 col-xs-12\"");
		//        if (idx < 0) break;
		//        s = s.Substring(idx);
		//
		//        idx = s.IndexOf("<a class=\"col-thumbnail\" href=\"") + "<a class=\"col-thumbnail\" href=\"..\\..\\".Length;
		//        s = s.Substring(idx);
		//
		//        idx = s.IndexOf("\"");
		//
		//        string url = "http://www.nudecollect.com/artistic_photos_1/" + s.Substring(0, idx);
		//
		//        Console.WriteLine(url);
		//
		//        Request(url, "e:\\temp\\nude\\1\\" + block + "\\", n + ".txt");
		//
		//        n++;
		//
		//    }
		//
		//}

		public static void Request(string url, string dir, string file)
		{
			FileStream fs = null;
			StreamWriter sw = null;
			try
			{
				if (!Directory.Exists(dir))
				{
					Directory.CreateDirectory(dir);
				}
				System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(url);
				System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
				System.IO.Stream st = myrp.GetResponseStream();
				StreamReader sr = new StreamReader(st, Encoding.GetEncoding("utf-8"));

				fs = new FileStream(dir + file, FileMode.Create, FileAccess.Write);
				sw = new StreamWriter(fs, Encoding.UTF8);

				while (true)
				{
					string line = sr.ReadLine();
					if (line == null) break;

					sw.WriteLine(line);
				}
				sr.Close();
				st.Close();
				myrp.Close();
				Myrq.Abort();
			}
			finally
			{
				sw.Flush();
				fs.Close();

			}
		}

		public static string Request(string url)
		{
			try
			{
				System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(url);
				System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
				System.IO.Stream st = myrp.GetResponseStream();
				StreamReader sr = new StreamReader(st, Encoding.GetEncoding("utf-8"));


				string s = sr.ReadToEnd();

				sr.Close();
				st.Close();
				myrp.Close();
				Myrq.Abort();
				return s;
			}
			finally
			{
			}
		}
		public static void RequestBin(string url, string dir, string file)
		{
			FileStream fs = null;
			try
			{
				if (!Directory.Exists(dir))
				{
					Directory.CreateDirectory(dir);
				}
				if (File.Exists(dir + file))
				{
					Console.WriteLine("exist:" + file);
					return;
				}
				System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(url);
				System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
				System.IO.Stream st = myrp.GetResponseStream();

				fs = new FileStream(dir + file, FileMode.Create, FileAccess.Write);

				while (true)
				{
					int d = st.ReadByte();
					if (d < 0) break;

					fs.WriteByte((byte)d);
				}
				st.Close();
				myrp.Close();
				Myrq.Abort();
			}
			catch (Exception ex)
			{
				Console.WriteLine("error:" + file);
			}
			finally
			{
				if (fs != null)
				{
					fs.Close();
				}
			}
		}

	}
}
