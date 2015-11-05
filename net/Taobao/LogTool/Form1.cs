using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;
using System.Threading;
using System.Net;
using System.Diagnostics;
using System.Runtime.Serialization.Json;

namespace LogTool
{
	public partial class Form1 : Form
	{

		public Form1()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
		}

		public void ShowPage(List<Taobao.Mods.ItemList.Data.Auction> data)
		{
			if (data.Count <= 0) return;
			string temp = System.IO.Path.GetTempFileName();
			string newfile = temp.Substring(0, temp.Length - 3) + "html";
			FileStream fs = new FileStream(newfile, FileMode.Create, FileAccess.Write);
			StreamWriter sw = new StreamWriter(fs);

			sw.WriteLine("<html>");
			sw.WriteLine("<head>");
			sw.WriteLine("<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />");
			sw.WriteLine("</head>");
			sw.WriteLine("<body>");

			sw.WriteLine("<script>");
			sw.WriteLine("function show(n)");
			sw.WriteLine("{");
			sw.WriteLine("  document.getElementById(n).src=document.getElementById(n).getAttribute(\"data\");");
			sw.WriteLine("  document.getElementById(n).height=100;");
			sw.WriteLine("  document.getElementById(n).width=100;");
			sw.WriteLine("}");
			sw.WriteLine("</script>");

			sw.WriteLine("<table border='1'>");
			string bg = "";

			foreach (var item in data)
			{
				sw.WriteLine("<tr" + bg + ">");

				sw.WriteLine("<td>");
				sw.WriteLine("<a href='http:" + item.detail_url + "' target='_blank'>" + item.raw_title + "</a>");
				sw.WriteLine("</td>");

				sw.WriteLine("<td>");
				//sw.WriteLine("<img src='http:" + item.pic_url + "'>");
				sw.WriteLine("<img id='n" + item.nid + "' data='http:" + item.pic_url + "'>");
				sw.WriteLine("</td>");

				sw.WriteLine("<td>");
				//sw.WriteLine(item.view_price);
				sw.WriteLine("<a href='javascript:show(\"n" + item.nid + "\")'>" + item.view_price + "</a>");
				sw.WriteLine("</td>");
				sw.WriteLine("<td>");
				sw.WriteLine(item.reserve_price);
				sw.WriteLine("</td>");
				sw.WriteLine("<td>");
				sw.WriteLine(item.view_fee);
				sw.WriteLine("</td>");

				sw.WriteLine("<td>");
				sw.WriteLine(item.item_loc);
				sw.WriteLine("</td>");

				sw.WriteLine("<td>");
				sw.WriteLine(item.comment_count);
				sw.WriteLine("</td>");

				sw.WriteLine("<td>");
				sw.WriteLine("<a href='http:" + item.shopcard.shopLink + "' target='_blank'>" + item.nick + "</a>");
				sw.WriteLine("</td>");

				sw.WriteLine("</tr>");

				if (bg == "")
				{
					bg = " style='background:#DDD;'";
				}
				else
				{
					bg = "";
				}

			}
			sw.WriteLine("</table>");
			sw.WriteLine("</body>");
			sw.WriteLine("</html>");

			sw.Flush();
			fs.Close();
			Process p = new Process();
			p.StartInfo = new ProcessStartInfo(newfile);
			p.Start();
		}
		List<Taobao.Mods.ItemList.Data.Auction> List1 = new List<Taobao.Mods.ItemList.Data.Auction>();
		List<Taobao.Mods.ItemList.Data.Auction> List2 = new List<Taobao.Mods.ItemList.Data.Auction>();
		List<Taobao.Mods.ItemList.Data.Auction> List3 = new List<Taobao.Mods.ItemList.Data.Auction>();
		List<Taobao.Mods.ItemList.Data.Auction> List4 = new List<Taobao.Mods.ItemList.Data.Auction>();

		private void button1_Click_1(object sender, EventArgs e)
		{
			List<Taobao.Mods.ItemList.Data.Auction> l1 = new List<Taobao.Mods.ItemList.Data.Auction>();
			List<Taobao.Mods.ItemList.Data.Auction> l2 = new List<Taobao.Mods.ItemList.Data.Auction>();
			List<Taobao.Mods.ItemList.Data.Auction> l3 = new List<Taobao.Mods.ItemList.Data.Auction>();
			List<Taobao.Mods.ItemList.Data.Auction> l4 = new List<Taobao.Mods.ItemList.Data.Auction>();
			
			l1.AddRange(List1);
			l2.AddRange(List2);
			l3.AddRange(List3);
			l4.AddRange(List4);

			l1 = filt(l1, l2, l3, l4);
			l2 = filt(l2, l1, l3, l4);
			l3 = filt(l3, l2, l1, l4);
			l4 = filt(l4, l2, l3, l1);

			ShowPage(l1);
			ShowPage(l2);
			ShowPage(l3);
			ShowPage(l4);
		}

		private void proc(string n, string t, List<Taobao.Mods.ItemList.Data.Auction> l)
		{
			if (!string.IsNullOrEmpty(n))
			{
				for (int i = 0; i < int.Parse(n); i++)
				{
					var tb = Request(t, i);
					l.AddRange(tb.mods.itemlist.data.auctions);
				}
			}
		}
		private List<Taobao.Mods.ItemList.Data.Auction> filt(List<Taobao.Mods.ItemList.Data.Auction> l, List<Taobao.Mods.ItemList.Data.Auction> l1, List<Taobao.Mods.ItemList.Data.Auction> l2, List<Taobao.Mods.ItemList.Data.Auction> l3)
		{
			List<Taobao.Mods.ItemList.Data.Auction> ret = new List<Taobao.Mods.ItemList.Data.Auction>();
			foreach (var item in l)
			{
				bool exist = false;
				if (l1.Count > 0)
				{
					foreach (var item1 in l1)
					{
						if (item1.nick == item.nick)
						{
							exist = true;
						}
					}
				}
				else
				{
					exist = true;
				}

				if (exist)
				{
					exist = false;
					if (l2.Count > 0)
					{
						foreach (var item2 in l2)
						{
							if (item2.nick == item.nick)
							{
								exist = true;
							}
						}
					}
					else
					{
						exist = true;
					}

				}
				if (exist)
				{
					exist = false;
					if (l3.Count > 0)
					{
						foreach (var item3 in l3)
						{
							if (item3.nick == item.nick)
							{
								exist = true;
							}
						}
					}
					else
					{
						exist = true;
					}

				}

				if (exist)
				{
					ret.Add(item);
				}
			}

			return ret;
		}

		[Serializable]
		public class Taobao
		{
			public Mods mods;
			public class Mods
			{
				public ItemList itemlist;

				public class ItemList
				{
					public Data data;

					public class Data
					{
						public List<Auction> auctions;

						public class Auction
						{
							public string nid;
							public string raw_title;
							public string nick;
							public string detail_url;
							public string pic_url;
							public string view_price;
							public string reserve_price;
							public string view_fee;
							public string item_loc;
							public string comment_count;
							public Shopcard shopcard;

							public class Shopcard
							{
								public string shopLink;
							}
						}
					}
				}
			}
		}

		public Taobao Request(string search, int no)
		{
			this.Text = search + " " + no;
			string _url;
			if (no == 0)
			{
				_url = "https://s.taobao.com/search?q=" + search + "&imgfile=&js=1&stats_click=search_radio_all%3A1&initiative_id=staobaoz_20151105&ie=utf8";
			}
			else if (no == 1)
			{
				_url = "https://s.taobao.com/search?q=" + search + "&imgfile=&js=1&stats_click=search_radio_all%3A1&initiative_id=staobaoz_20151105&ie=utf8&bcoffset=0&p4plefttype=3%2C1&p4pleftnum=1%2C3&s=44";
			}
			else
			{
				_url = "https://s.taobao.com/search?q=" + search + "&imgfile=&js=1&stats_click=search_radio_all%3A1&initiative_id=staobaoz_20151105&ie=utf8&bcoffset=-3&p4plefttype=3%2C1&p4pleftnum=1%2C3&ntoffset=-3&s=" + (no * 44);
			}
			System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(_url);
			System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
			System.IO.Stream st = myrp.GetResponseStream();
			StreamReader sr = new StreamReader(st);
			while (true)
			{
				string line = sr.ReadLine();
				if (line == null) break;
				if (line.Trim().StartsWith("g_page_config"))
				{
					sr.Close();
					st.Close();
					myrp.Close();
					Myrq.Abort();
					return DeSerial(line.Trim().Substring("g_page_config = ".Length).TrimEnd(';'));
				}
			}

			sr.Close();
			st.Close();
			myrp.Close();
			Myrq.Abort();
			return null;
		}
		private Taobao DeSerial(string jsonString)
		{
			DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(Taobao));
			MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(jsonString));
			Taobao obj = (Taobao)ser.ReadObject(ms);
			return obj;
		}

		private void button2_Click(object sender, EventArgs e)
		{
			proc(n1.Text, t1.Text, List1);
		}

		private void button3_Click(object sender, EventArgs e)
		{
			proc(n2.Text, t2.Text, List2);
		}

		private void button4_Click(object sender, EventArgs e)
		{
			proc(n3.Text, t3.Text, List3);
		}

		private void button5_Click(object sender, EventArgs e)
		{
			proc(n4.Text, t4.Text, List4);
		}

	}

}
