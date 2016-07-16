using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace proc
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			FileStream fs = new FileStream(@"D:\字幕.srt", FileMode.Open, FileAccess.Read);
			StreamReader sr = new StreamReader(fs);

			FileStream fw = new FileStream(@"D:\字幕out.srt", FileMode.Create, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fw);

			string s = sr.ReadToEnd();
			fs.Close();

			string[] sep = new string[1];
			sep[0] = "\r\n\r\n";
			string[] sep2 = new string[1];
			sep2[0] = " --> ";
			string[] ss = s.Split(sep, StringSplitOptions.None);
			foreach (var item in ss)
			{
				string proc = item;

				if (proc != "")
				{
					string no = proc.Substring(0, proc.IndexOf("\r\n"));
					sw.WriteLine(no);
					proc = proc.Substring(proc.IndexOf("\r\n") + 2);
					string time = proc.Substring(0, proc.IndexOf("\r\n"));
					TimeSpan time1 = TimeSpan.Parse(time.Split(sep2, StringSplitOptions.None)[0].Replace(",", "."));
					TimeSpan time2 = TimeSpan.Parse(time.Split(sep2, StringSplitOptions.None)[1].Replace(",", "."));
					TimeSpan dif = new TimeSpan(0, 0, -59);
					time1 = time1.Add(dif);
					time2 = time2.Add(dif);

					string t1 = time1.ToString("G").Substring(2, 12).Replace(".", ",");
					string t2 = time2.ToString("G").Substring(2, 12).Replace(".", ",");
					sw.WriteLine(t1 + " --> " + t2);


					string rest = proc.Substring(proc.IndexOf("\r\n") + 2);
					sw.WriteLine(rest.Replace("</i>", ""));
					sw.Write("\r\n");
				}
			}
			sw.Flush();
			fw.Close();
		}
	}
}
